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
#pragma once

#define WIN_PATH_SEPARATOR "\\"
#define POSIX_PATH_SEPARATOR "/"

#if defined(_WIN32)
#define PATH_SEPARATOR WIN_PATH_SEPARATOR
#else
#define PATH_SEPARATOR POSIX_PATH_SEPARATOR
#endif

#define PATH_SELF "."
#define PATH_PARENT ".."

int posix_to_win_path(char * path);
int win_to_posix_path(char * path);

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-function"
#pragma GCC diagnostic ignored "-Wunused-parameter"

static int host_to_win_path(char * path) {
#if defined(_WIN32)
    return 0;
#else
    return posix_to_win_path(path);
#endif
}

static int win_to_host_path(char * path) {
#if defined(_WIN32)
    return 0;
#else
    return win_to_posix_path(path);
#endif
}

static int host_to_posix_path(char * path) {
#if defined(_WIN32)
    return win_to_posix_path(path);
#else
    return 0;
#endif
}

static int posix_to_host_path(char * path) {
#if defined(_WIN32)
    return posix_to_win_path(path);
#else
    return 0;
#endif
}

#pragma GCC diagnostic pop

int parent_path(char * path);
int base_path(char * path);
