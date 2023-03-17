#ifndef _TIME_H
#include <time/time.h>

#ifndef _ISOMAC
# include <bits/types/locale_t.h>

extern __typeof (strftime_l) __strftime_l;
#ifndef LIBCOMPATCOLL_MODE
libc_hidden_proto (__strftime_l)
#endif /* LIBCOMPATCOLL_MODE */
extern __typeof (strptime_l) __strptime_l;

#ifndef LIBCOMPATCOLL_MODE
libc_hidden_proto (time)
#endif /* LIBCOMPATCOLL_MODE */
libc_hidden_proto (asctime)
#ifndef LIBCOMPATCOLL_MODE
libc_hidden_proto (mktime)
libc_hidden_proto (timelocal)
libc_hidden_proto (localtime)
#endif /* LIBCOMPATCOLL_MODE */
libc_hidden_proto (strftime)
#ifndef LIBCOMPATCOLL_MODE
libc_hidden_proto (strptime)
#endif /* LIBCOMPATCOLL_MODE */

extern __typeof (clock_getres) __clock_getres;
extern __typeof (clock_gettime) __clock_gettime;
libc_hidden_proto (__clock_gettime)
extern __typeof (clock_settime) __clock_settime;
extern __typeof (clock_nanosleep) __clock_nanosleep;
extern __typeof (clock_getcpuclockid) __clock_getcpuclockid;

/* Now define the internal interfaces.  */
struct tm;

/* Defined in mktime.c.  */
extern const unsigned short int __mon_yday[2][13] attribute_hidden;

/* Defined in localtime.c.  */
extern struct tm _tmbuf attribute_hidden;

/* Defined in tzset.c.  */
extern char *__tzstring (const char *string);

extern int __use_tzfile attribute_hidden;

extern void __tzfile_read (const char *file, size_t extra,
			   char **extrap);
extern void __tzfile_compute (time_t timer, int use_localtime,
			      long int *leap_correct, int *leap_hit,
			      struct tm *tp);
extern void __tzfile_default (const char *std, const char *dst,
			      long int stdoff, long int dstoff);
extern void __tzset_parse_tz (const char *tz);
extern void __tz_compute (time_t timer, struct tm *tm, int use_localtime)
     __THROW internal_function;

/* Subroutine of `mktime'.  Return the `time_t' representation of TP and
   normalize TP, given that a `struct tm *' maps to a `time_t' as performed
   by FUNC.  Keep track of next guess for time_t offset in *OFFSET.  */
extern time_t __mktime_internal (struct tm *__tp,
				 struct tm *(*__func) (const time_t *,
						       struct tm *),
				 time_t *__offset);
#ifndef LIBCOMPATCOLL_MODE
extern struct tm *__localtime_r (const time_t *__timer,
				 struct tm *__tp) attribute_hidden;
#else
extern struct tm *__localtime_r (const time_t *__timer,
                                 struct tm *__tp);
#endif /* LIBCOMPATCOLL_MODE */


extern struct tm *__gmtime_r (const time_t *__restrict __timer,
			      struct tm *__restrict __tp);
libc_hidden_proto (__gmtime_r)

/* Compute the `struct tm' representation of *T,
   offset OFFSET seconds east of UTC,
   and store year, yday, mon, mday, wday, hour, min, sec into *TP.
   Return nonzero if successful.  */
extern int __offtime (const time_t *__timer,
		      long int __offset,
		      struct tm *__tp);

extern char *__asctime_r (const struct tm *__tp, char *__buf);
extern void __tzset (void);

/* Prototype for the internal function to get information based on TZ.  */
extern struct tm *__tz_convert (const time_t *timer, int use_localtime, struct tm *tp);

extern int __nanosleep (const struct timespec *__requested_time,
			struct timespec *__remaining);
hidden_proto (__nanosleep)
extern int __nanosleep_nocancel (const struct timespec *__requested_time,
				 struct timespec *__remaining)
  attribute_hidden;
extern int __getdate_r (const char *__string, struct tm *__resbufp);


/* Determine CLK_TCK value.  */
extern int __getclktck (void);


/* strptime support.  */
extern char * __strptime_internal (const char *rp, const char *fmt,
				   struct tm *tm, void *statep,
				   locale_t locparam)
     internal_function;

extern double __difftime (time_t time1, time_t time0);


/* Use in the clock_* functions.  Size of the field representing the
   actual clock ID.  */
#define CLOCK_IDFIELD_SIZE	3

#endif
#endif
