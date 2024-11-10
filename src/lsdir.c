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

#if defined(_WIN32)
#error not supported
#else


static void free_directory_list_on_fail(struct directory_entry * list, DIR * dir, void * extra) {
    int err = errno;
    if(extra) free(extra);
    if(list) free_directory_list(list);
    if(dir) closedir(dir);
    errno = err;
}

int list_directory(const char * path, struct directory_entry ** listptr) {
    struct directory_entry * list = NULL;

    DIR * dir = opendir(path);
    if(!dir) return -1;
    for(struct directory_entry * curr = NULL;;) {
        errno = 0;
        struct dirent * ent = readdir(dir);
        if(!ent) {
            if(errno == 0) {
                break;
            } else {
                free_directory_list_on_fail(list, dir, NULL);
                return -1;
            }
        } else {
            if(strcmp(ent->d_name, PATH_SELF) == 0 || strcmp(ent->d_name, PATH_PARENT) == 0) continue;

            struct directory_entry * next = malloc(sizeof(struct directory_entry));
            if(!next) {
                free_directory_list_on_fail(list, dir, NULL);
                return -1;
            }
            next->path = malloc(strlen(path) + strlen(PATH_SEPARATOR) + strlen(ent->d_name) + 1);
            if(!next->path) {
                free_directory_list_on_fail(list, dir, next);
                return -1;
            }
            next->next = NULL;
            next->prev = curr;

            if(curr != NULL) {
                curr->next = next;
            } else {
                list = next;
            }

            strcpy(next->path, path);
            strcat(next->path, PATH_SEPARATOR);
            strcat(next->path, ent->d_name);

            curr = next;
        }
    }
    if(closedir(dir) != 0) {
        free_directory_list_on_fail(list, NULL, NULL);
        return -1;
    }
    *listptr = list;
    return 0;
}


#endif

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
