/* Return backtrace of current program state.
   Copyright (C) 1998, 2000, 2002, 2005 Free Software Foundation, Inc.
   This file is part of the GNU C Library.

   The GNU C Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Library General Public License as
   published by the Free Software Foundation; either version 2 of the
   License, or (at your option) any later version.

   The GNU C Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public
   License along with the GNU C Library; see the file COPYING.LIB.  If
   not, see <http://www.gnu.org/licenses/>.  */

#include <execinfo.h>
#include <stddef.h>

/* This is the stack layout we see with every stack frame.
   Note that every routine is required by the ABI to lay out the stack
   like this.

            +----------------+        +-----------------+
    %r1  -> | %r1 last frame--------> | %r1 last frame--->...  --> NULL
            |                |        |                 |
            | cr save        |        | cr save	  |
            |                |        |                 |
            | (unused)       |        | return address  |
            +----------------+        +-----------------+
*/
struct layout
{
  struct layout *next;
  long condition_register;
  void *return_address;
};

int
__backtrace (void **array, int size)
{
  struct layout *current;
  int count;

  /* Force gcc to spill LR.  */
  asm volatile ("" : "=l"(current));

  /* Get the address on top-of-stack.  */
  asm volatile ("ld %0,0(1)" : "=r"(current));

  for (				count = 0;
       current != NULL && 	count < size;
       current = current->next, count++)
    array[count] = current->return_address;

  /* It's possible the second-last stack frame can't return
     (that is, it's __libc_start_main), in which case
     the CRT startup code will have set its LR to 'NULL'.  */
  if (count > 0 && array[count-1] == NULL)
    count--;

  return count;
}
weak_alias (__backtrace, backtrace)
libc_hidden_def (__backtrace)