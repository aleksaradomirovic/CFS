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

int parent_path(char * path) {
    for(char * sep = path;;) {
        char * nsep = strstr(sep + strlen(PATH_SEPARATOR), PATH_SEPARATOR);
        if(nsep == NULL) {
            if(sep != path) *sep = '\0';
            break;
        }
        sep = nsep;
    }
    return 0;
}
