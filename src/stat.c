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
#include "cfs/stat.h"

#include <errno.h>

#include <sys/types.h>
#include <sys/stat.h>

#if defined(_WIN32)
#define STAT _stat
#else
#define STAT stat
#endif

static int __exists(const char * path, struct STAT * ent) {
    int err = errno;
    if(STAT(path, ent) != 0) {
        if(errno == ENOENT) {
            errno = err;
            return 0;
        } else {
            return -1;
        }
    } else {
        return 1;
    }
}

int exists(const char * path) {
    struct STAT ent;
    return __exists(path, &ent);
}

int is_regular_file(const char * path) {
    struct STAT ent;
    int status = __exists(path, &ent);
    if(status != 1) return status;

    return S_ISREG(ent.st_mode) ? 1 : 0;
}

int is_directory(const char * path) {
    struct STAT ent;
    int status = __exists(path, &ent);
    if(status != 1) return status;

    return S_ISDIR(ent.st_mode) ? 1 : 0;
}
