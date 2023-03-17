/* Frees the dynamically allocated storage from an earlier call to glob.
   Linux version.
   Copyright (C) 2017 Free Software Foundation, Inc.
   This file is part of the GNU C Library.

   The GNU C Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.

   The GNU C Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with the GNU C Library; if not, see
   <http://www.gnu.org/licenses/>.  */

#include <sys/stat.h>
#include <kernel_stat.h>

#define globfree64 __no_globfree64_decl
#include <posix/globfree.c>
#undef globfree64

#if XSTAT_IS_XSTAT64
weak_alias (globfree, globfree64)
libc_hidden_ver (globfree, globfree64)
#endif
