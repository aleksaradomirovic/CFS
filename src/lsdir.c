/*
 * CFS is a C-only filesystem library.
 * Copyright (C) 2024  Aleksa Radomirovic
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, see <https://www.gnu.org/licenses/>.
 */
#include "cfs/dir.h"
#include "cfs/path.h"
#include "cfs/stat.h"

#include <errno.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#if defined(_WIN32)
#include <fileapi.h>
#else
#include <dirent.h>
#endif

void free_directory_list(struct directory_entry * list) {
    for(struct directory_entry * ent = list; ent != NULL;) {
        struct directory_entry * next = ent->next;
        free(ent->path);
        free(ent);
        ent = next;
    }
}

static int read_directory_entry(DIR * dir, const char * rootpath, struct directory_entry ** entptr) {
    errno = 0;
    struct dirent * osent = readdir(dir);
    if(osent == NULL) {
        if(errno == 0) {
            *entptr = NULL;
            return 0;
        } else {
            return -1;
        }
    }

    size_t pathlen = strlen(rootpath);
    if(
        __builtin_add_overflow(pathlen, strlen(PATH_SEPARATOR), &pathlen) ||
        __builtin_add_overflow(pathlen, strlen(osent->d_name), &pathlen) ||
        __builtin_add_overflow(pathlen, 1, &pathlen)
    ) {
        errno = ENAMETOOLONG;
        return -1;
    }

    struct directory_entry * ent = malloc(sizeof(struct directory_entry));
    if(!ent) return -1;

    ent->path = malloc(pathlen);
    if(!ent->path) {
        int err = errno;
        free(ent);
        errno = err;
        return -1;
    }

    strcpy(ent->path, rootpath);
    strcat(ent->path, PATH_SEPARATOR);
    strcat(ent->path, osent->d_name);

    ent->next = NULL;
    ent->prev = NULL;

    *entptr = ent;
    return 0;
}

static int check_keep_name(const struct directory_entry * ent) {
    char * path = malloc(strlen(ent->path) + 1);
    if(!path) return -1;

    strcpy(path, ent->path);

    if(base_path(path) != 0) {
        int err = errno;
        free(path);
        errno = err;
        return -1;
    }

    if(
        strcmp(path, PATH_SELF) == 0 ||
        strcmp(path, PATH_PARENT) == 0
    ) {
        free(path);
        return 0;
    }

    free(path);
    return 1;
}

int list_directory(const char * path, struct directory_entry ** listptr) {
    int status;

    struct directory_entry * list = NULL;
    
    DIR * dir = opendir(path);
    if(!dir) return -1;

    for(struct directory_entry * curr = NULL;;) {
        struct directory_entry * next;
        status = read_directory_entry(dir, path, &next);
        if(status != 0) {
            int err = errno;
            free_directory_list(list);
            errno = err;
            return -1;
        }
        if(next == NULL) break;

        status = check_keep_name(next);
        if(status < 0) {
            int err = errno;
            free_directory_list(list);
            free_directory_list(next);
            errno = err;
            return -1;
        } else if(status == 0) {
            free_directory_list(next);
            continue;
        }

        next->prev = curr;
        if(curr != NULL) {
            curr->next = next;
        } else {
            list = next;
        }
        curr = next;
    }

    status = closedir(dir);
    if(status != 0) {
        int err = errno;
        free_directory_list(list);
        errno = err;
        return -1;
    }

    *listptr = list;
    return 0;
}

static struct directory_entry * directory_list_end(struct directory_entry * list) {
    for(; list->next != NULL; list = list->next);
    return list;
}

int list_directory_recursive(const char * path, struct directory_entry ** listptr) {
    int status;

    struct directory_entry * list;
    status = list_directory(path, &list);
    if(status != 0) return -1;

    if(list != NULL) {
        for(struct directory_entry * ent = list; ent != NULL; ent = ent->next) {
            status = is_directory(ent->path);
            if(status == 1) {
                struct directory_entry * sublist;
                status = list_directory(ent->path, &sublist);
                if(status != 0) {
                    int err = errno;
                    free_directory_list(list);
                    errno = err;
                    return -1;
                }

                if(sublist != NULL) {
                    sublist->prev = ent;
                    directory_list_end(sublist)->next = ent->next;
                    ent->next = sublist;
                }
            } else if(status != 0) {
                int err = errno;
                free_directory_list(list);
                errno = err;
                return -1;
            }
        }
    }

    *listptr = list;
    return 0;
}
