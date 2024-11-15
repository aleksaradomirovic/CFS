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

#include <errno.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#if defined(_WIN32)
#define GETCWD _getcwd
#define CHDIR _chdir
#else
#define GETCWD getcwd
#define CHDIR chdir
#endif

int get_current_directory(char ** dir) {
    char * cwd = malloc(PATH_MAX);
    if(!cwd) return -1;

    if(!GETCWD(cwd, PATH_MAX)) {
        int err = errno;
        free(cwd);
        errno = err;
        return -1;
    }

    {
        char * trim = realloc(cwd, strnlen(cwd, PATH_MAX));
        if(trim) {
            cwd = trim;
        }
    }

    *dir = cwd;
    return 0;
}

int set_current_directory(const char * dir) {
    int status;
    status = CHDIR(dir);
    if(status != 0) return -1;
    return 0;
}
