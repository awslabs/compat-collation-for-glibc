/* Copyright (C) 1995-2002, 2004 Free Software Foundation, Inc.
   This file is part of the GNU C Library.
   Written by Ulrich Drepper <drepper@cygnus.com>, 1995.

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

#include <string.h>

#ifndef STRING_TYPE
# define STRING_TYPE char
# define STRCOLL strcoll
# define STRCOLL_L __strcoll_l
# define USE_HIDDEN_DEF
#ifdef LIBCOMPATCOLL_MODE
#define STRCOLLFUNC
#endif /* LIBCOMPATCOLL_MODE */
#endif

#include "../locale/localeinfo.h"

#if defined LIBCOMPATCOLL_MODE && defined STRCOLLFUNC
#include <dlfcn.h>
#include <stdio.h>

static int (*strcoll_l_glibc)(const STRING_TYPE *s1, const STRING_TYPE *s2, locale_t l);
static int check_glibc_strcoll = 0;

void __attribute__ ((constructor))
strcoll_l_glibc_init(void)
{
        strcoll_l_glibc = dlsym(RTLD_NEXT, "strcoll_l");
}

void
set_check_glibc_strcoll(void)
{
  check_glibc_strcoll = 1;
}

void
unset_check_glibc_strcoll(void)
{
  check_glibc_strcoll = 0;
}
#endif /* LIBCOMPATCOLL_MODE */

int
STRCOLL (const STRING_TYPE *s1, const STRING_TYPE *s2)
{
#ifndef LIBCOMPATCOLL_MODE
  return STRCOLL_L (s1, s2, _NL_CURRENT_LOCALE);
#else
#ifdef STRCOLLFUNC
  int  retl = STRCOLL_L (s1, s2, _NL_CURRENT_LOCALE);

  if (__glibc_unlikely(check_glibc_strcoll))
  {
    int   retg = strcoll_l_glibc(s1, s2, _NL_CURRENT_LOCALE);

    if (((retg > 0) - (retg < 0)) != ((retl > 0) - (retl < 0)))
      fprintf(stderr, "strcoll mismatch s1 = \"%s\", s2 = \"%s\": compat = %d, glibc = %d\n",
              s1, s2, retl, retg);
  }

  return retl;
#else
  return STRCOLL_L (s1, s2, _NL_CURRENT_LOCALE);
#endif /* STRCOLLFUNC */
#endif /* LIBCOMPATCOLL_MODE */
}

#ifndef LIBCOMPATCOLL_MODE
#ifdef USE_HIDDEN_DEF
libc_hidden_def (STRCOLL)
#endif
#endif /* LIBCOMPATCOLL_MODE */
