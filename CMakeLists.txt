# CFS is a C-only filesystem library.
# Copyright (C) 2024  Aleksa Radomirovic
#
# This library is free software; you can redistribute it and/or
# modify it under the terms of the GNU Lesser General Public
# License as published by the Free Software Foundation; either
# version 2.1 of the License, or (at your option) any later version.
#
# This library is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# Lesser General Public License for more details.
#
# You should have received a copy of the GNU Lesser General Public
# License along with this library; if not, see <https://www.gnu.org/licenses/>.

cmake_minimum_required(VERSION 3.25)
project(cfs LANGUAGES C)

add_compile_options(
    -std=gnu17
    -O2
    -fanalyzer
    -Wall -Wextra
)

set(
    CFS_SRC
    src/lsdir.c
    src/mkdir.c
    src/stat.c
)

add_library(
    cfs-static STATIC ${CFS_SRC}
)
target_include_directories(
    cfs-static PUBLIC include
)
set_property(
    TARGET cfs-static
    PROPERTY OUTPUT_NAME cfs
)

add_library(
    cfs-shared SHARED ${CFS_SRC}
)
target_include_directories(
    cfs-shared PUBLIC include
)
set_property(
    TARGET cfs-shared
    PROPERTY OUTPUT_NAME cfs
)
