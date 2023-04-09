#ifndef _ERRNO_H

#include <stdlib/errno.h>

#if defined _ERRNO_H && !defined _ISOMAC && !defined __cplusplus

# if IS_IN (rtld)
#  include <dl-sysdep.h>
#  ifndef RTLD_PRIVATE_ERRNO
#   error "dl-sysdep.h must define RTLD_PRIVATE_ERRNO!"
#  endif
# else
#  define RTLD_PRIVATE_ERRNO	0
# endif

# if RTLD_PRIVATE_ERRNO
/* The dynamic linker uses its own private errno variable.
   All access to errno inside the dynamic linker is serialized,
   so a single (hidden) global variable is all it needs.  */

#  undef  errno
#  define errno rtld_errno
extern int rtld_errno attribute_hidden;

# elif IS_IN_LIB

#  include <tls.h>

#  undef  errno
#  if IS_IN (libc)
#ifndef LIBCOMPATCOLL_MODE
#   define errno __libc_errno
#else
#   define errno errno
#endif /* LIBCOMPATCOLL_MODE */
#  else
#   define errno errno		/* For #ifndef errno tests.  */
#  endif
extern __thread int errno attribute_tls_model_ie;

# endif	/* IS_IN_LIB */

# define __set_errno(val) (errno = (val))

# ifndef __ASSEMBLER__
extern int *__errno_location (void) __THROW __attribute__ ((__const__))
#  if RTLD_PRIVATE_ERRNO
     attribute_hidden
#  endif
;
#ifndef LIBCOMPATCOLL_MODE
libc_hidden_proto (__errno_location)
#endif /* LIBCOMPATCOLL_MODE */
# endif

#endif /* _ERRNO_H */

#endif /* ! _ERRNO_H */
