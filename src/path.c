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

#include <string.h>

static char * __find_last_separator(const char * path) {
    for(const char * sep = path;;) {
        const char * nsep = strstr(sep, PATH_SEPARATOR);
        if(nsep == NULL) {
            if(sep == path) return NULL;
            return (char *) sep;
        }
        sep = nsep + strlen(PATH_SEPARATOR);
    }
}

int parent_path(char * path) {
    char * sep = __find_last_separator(path);
    if(sep != NULL) {
        *sep = '\0';
    } else {
    	*path = '\0';
    }

    return 0;
}

int base_path(char * path) {
    char * sep = __find_last_separator(path);
    if(sep != NULL) {
        strcpy(path, sep);
    }

    return 0;
}
