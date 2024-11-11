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
#include <limits.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#if defined(_WIN32)
#include <direct.h>
#endif

int create_directory_with_perms(const char * path, file_perms_t perms) {
#if defined(_WIN32)
    errno = ENOTSUP;
    return -1;
#else
    int status = mkdir(path, perms & 0777);
    if(status != 0) return -1;
    return 0;
#endif
}

int create_directory(const char * path) {
    int status;
#if defined(_WIN32)
    status = _mkdir(path);
#else
    status = create_directory_with_perms(path, 0777);
#endif
    if(status != 0) return -1;
    return 0;
}

int create_directory_recursive(const char * pathstr) {
    int status;

    if(strlen(pathstr) + 1 > PATH_MAX) {
        errno = ENAMETOOLONG;
        return -1;
    }

    char * path = malloc(strlen(pathstr) + 1);
    if(!path) return -1;
    strcpy(path, pathstr);

    char * sep = path;
    while(1) {
        sep = strstr(sep, PATH_SEPARATOR);
        if(sep == NULL) break;

        sep += strlen(PATH_SEPARATOR);
        char c = *sep;
        *sep = '\0';

        status = is_directory(path);
        if(status < 0) {
            int err = errno;
            free(path);
            errno = err;
            return -1;
        }
        if(status == 0) {
            status = create_directory(path);
            if(status != 0) {
                int err = errno;
                free(path);
                errno = err;
                return -1;
            }
        }

        *sep = c;
    }

    free(path);
    return 0;
}
