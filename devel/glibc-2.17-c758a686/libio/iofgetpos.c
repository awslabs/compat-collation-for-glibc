/* Copyright (C) 1993-2012 Free Software Foundation, Inc.
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
   <http://www.gnu.org/licenses/>.

   As a special exception, if you link the code in this file with
   files compiled with a GNU compiler to produce an executable,
   that does not cause the resulting executable to be covered by
   the GNU Lesser General Public License.  This exception does not
   however invalidate any other reasons why the executable file
   might be covered by the GNU Lesser General Public License.
   This exception applies to code released by its copyright holders
   in files containing the exception.  */

/* We need to avoid the header declarations of these, because
   the types don't match _IO_fgetpos and then the compiler will
   complain about the mismatch when we do the alias below.  */
#define _IO_new_fgetpos64 __renamed__IO_new_fgetpos64
#define _IO_fgetpos64 __renamed__IO_fgetpos64

#include "libioP.h"

#undef _IO_new_fgetpos64
#undef _IO_fgetpos64

#include <errno.h>
#include <stdlib.h>
#include <shlib-compat.h>

int
_IO_new_fgetpos (_IO_FILE *fp, _IO_fpos_t *posp)
{
  _IO_off64_t pos;
  int result = 0;
  CHECK_FILE (fp, EOF);
  _IO_acquire_lock (fp);
  pos = _IO_seekoff_unlocked (fp, 0, _IO_seek_cur, 0);
  if (_IO_in_backup (fp) && pos != _IO_pos_BAD)
    {
      if (fp->_mode <= 0)
	pos -= fp->_IO_save_end - fp->_IO_save_base;
    }
  if (pos == _IO_pos_BAD)
    {
      /* ANSI explicitly requires setting errno to a positive value on
	 failure.  */
#ifdef EIO
      if (errno == 0)
	__set_errno (EIO);
#endif
      result = EOF;
    }
  else if ((_IO_off64_t) (__typeof (posp->__pos)) pos != pos)
    {
#ifdef EOVERFLOW
      __set_errno (EOVERFLOW);
#endif
      result = EOF;
    }
  else
    {
      posp->__pos = pos;
      if (fp->_mode > 0
	  && (*fp->_codecvt->__codecvt_do_encoding) (fp->_codecvt) < 0)
	/* This is a stateful encoding, safe the state.  */
	posp->__state = fp->_wide_data->_IO_state;
    }

  _IO_release_lock (fp);
  return result;
}

strong_alias (_IO_new_fgetpos, __new_fgetpos)
versioned_symbol (libc, _IO_new_fgetpos, _IO_fgetpos, GLIBC_2_2);
versioned_symbol (libc, __new_fgetpos, fgetpos, GLIBC_2_2);

#ifdef __OFF_T_MATCHES_OFF64_T
strong_alias (_IO_new_fgetpos, _IO_new_fgetpos64)
strong_alias (_IO_new_fgetpos64, __new_fgetpos64)
versioned_symbol (libc, _IO_new_fgetpos64, _IO_fgetpos64, GLIBC_2_2);
versioned_symbol (libc, __new_fgetpos64, fgetpos64, GLIBC_2_2);
#endif
