/* Copyright (C) 1992-2017 Free Software Foundation, Inc.
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

#include "version.h"
#include <tls.h>
#include <libc-abis.h>
#include <gnu/libc-version.h>

#ifdef LIBCOMPATCOLL_MODE
static const char __libc_release[] = RELEASE " (compatcollation)";
static const char __libc_version[] = VERSION "-" GLIBCRELEASE;

/* the following copied from elf/rtld.c to support aarch64 */
#ifndef THREAD_SET_STACK_GUARD
/* Only exported for architectures that don't store the stack guard canary
   in thread local area.  */
uintptr_t __stack_chk_guard attribute_relro;
#endif

/* Only exported for architectures that don't store the pointer guard
   value in thread local area.  */
uintptr_t __pointer_chk_guard_local
     attribute_relro attribute_hidden __attribute__ ((nocommon));
#ifndef THREAD_SET_POINTER_GUARD
strong_alias (__pointer_chk_guard_local, __pointer_chk_guard)
#endif

#else
static const char __libc_release[] = RELEASE;
static const char __libc_version[] = VERSION;
#endif /* LIBCOMPATCOLL_MODE */

#ifndef LIBCOMPATCOLL_MODE
static const char banner[] =
"GNU C Library "PKGVERSION RELEASE" release version "VERSION", by Roland McGrath et al.\n\
Copyright (C) 2017 Free Software Foundation, Inc.\n\
This is free software; see the source for copying conditions.\n\
There is NO warranty; not even for MERCHANTABILITY or FITNESS FOR A\n\
PARTICULAR PURPOSE.\n\
Compiled by GNU CC version "__VERSION__".\n"
#include "version-info.h"
#ifdef LIBC_ABIS_STRING
LIBC_ABIS_STRING
#endif
"For bug reporting instructions, please see:\n\
"REPORT_BUGS_TO".\n";

#include <unistd.h>

extern void __libc_print_version (void);
void
__libc_print_version (void)
{
  __write (STDOUT_FILENO, banner, sizeof banner - 1);
}
#endif /* LIBCOMPATCOLL_MODE */

extern const char *__gnu_get_libc_release (void);
const char *
__gnu_get_libc_release (void)
{
  return __libc_release;
}
weak_alias (__gnu_get_libc_release, gnu_get_libc_release)

extern const char *__gnu_get_libc_version (void);
const char *
__gnu_get_libc_version (void)
{
  return __libc_version;
}
weak_alias (__gnu_get_libc_version, gnu_get_libc_version)

/* This function is the entry point for the shared object.
   Running the library as a program will get here.  */

#ifndef LIBCOMPATCOLL_MODE
extern void __libc_main (void) __attribute__ ((noreturn));
void
__libc_main (void)
{
  __libc_print_version ();
  _exit (0);
}
#endif /* LIBCOMPATCOLL_MODE */
