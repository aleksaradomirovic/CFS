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

#include <sys/stat.h>

#if defined(_WIN32)
#include <direct.h>
#endif

int create_directory_with_perms(const char * path, file_perms_t perms) {
    int status;
#if defined(_WIN32)
    status = _mkdir(path);
#else
    status = mkdir(path, perms & 0777);
#endif
    if(status != 0) return -1;
    return 0;
}

int create_directory(const char * path) {
    return create_directory_with_perms(path, 0777);
}
