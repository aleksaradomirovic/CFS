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

#include <stdint.h>

typedef uint_least16_t file_perms_t;

int create_directory(const char * path);
int create_directory_with_perms(const char * path, file_perms_t perms);

struct directory_entry {
    char * path;
    struct directory_entry * next;
};

void free_directory_list(struct directory_entry * list);
int list_directory(const char * path, struct directory_entry ** listptr);
int list_directory_recursive(const char * path, struct directory_entry ** listptr);
