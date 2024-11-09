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

/**
 * Basic status queries
 * Arguments:
 *   const char * path - the path to query
 * Return values:
 *   1 if query is true
 *   0 if query is false
 *  -1 if error occurred
 */

int exists(const char * path);          // does this path exist?
int is_regular_file(const char * path); // does this path exist and is it a regular file?
int is_directory(const char * path);    // does this path exist and is it a directory?
