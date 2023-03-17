/* Copyright (C) 2013 Free Software Foundation, Inc.
   This file is part of the GNU C Library.
   Contributed by Ulrich Drepper <drepper@redhat.com>, 2002.

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

/* The standard design pattern for making it optional to link against
   libpthread is to mark the function weak, test if the function
   address is non-zero and call the function, otherwise use a fallback.
   The problem with pthread_atfork is that there is no viable
   fallback.  If you need to do something during fork it has to be done
   via pthread_atfork.  This makes having libpthread optional and using
   pthread_atfork impossible.  We make it possible by providing
   pthread_atfork in libc_nonshared.a.  The real work of pthread_atfork
   is done by __register_atfork which is already provided in
   libc_nonshared.a.  It's included in libc_nonshared.a because
   __dso_handle has to be unique to each DSO such that unloading the DSO
   can unregister the atfork handlers.  We build pthread_atfork again
   under a different file name and include it into libc_nonshared.a and
   libc.a. We keep pthread_atfork in libpthread_nonshared.a and
   libpthread.a for compatibility and completeness.

   Applications that can't rely on a new glibc should use the following
   code to optionally include libpthread and still register a function
   via pthread_atfork i.e. use __register_atfork directly:

   extern void *__dso_handle __attribute__ ((__weak__, __visibility__ ("hidden")));
   extern int __register_atfork (void (*) (void), void (*) (void), void (*) (void), void *);

   static int __app_atfork (void (*prepare) (void), void (*parent) (void), void (*child) (void))
     {
       return __register_atfork (prepare, parent, child,
				 &__dso_handle == NULL ? NULL : __dso_handle);
     }

   This code requires glibc 2.3.2 or newer. Previous to 2.3.2 no such
   interfaces exist and at that point is is impossible to have an
   optional libpthread and call pthread_atfork.

   This code adds no more ABI requirements than already exist since
   __dso_handle and __register_atfork are already part of the ABI.  */
#include <pthread_atfork.c>
