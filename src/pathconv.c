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
#include "cfs/path.h"

#include <stdlib.h>
#include <string.h>

// !!! ONLY VALID IF strlen(osep) == strlen(nsep)
static void translate_path_simple(char * path, const char * osep, const char * nsep) {
    for(char * sep = path;;) {
        sep = strstr(sep, osep);
        if(sep == NULL) break;
        memcpy(sep, nsep, strlen(osep));
        sep += strlen(osep);
    }
}

int posix_to_win_path(char * path) {
    translate_path_simple(path, POSIX_PATH_SEPARATOR, WIN_PATH_SEPARATOR);
    return 0;
}

int win_to_posix_path(char * path) {
    translate_path_simple(path, WIN_PATH_SEPARATOR, POSIX_PATH_SEPARATOR);
    return 0;
}
