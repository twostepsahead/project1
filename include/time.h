/*
 * CDDL HEADER START
 *
 * The contents of this file are subject to the terms of the
 * Common Development and Distribution License (the "License").
 * You may not use this file except in compliance with the License.
 *
 * You can obtain a copy of the license at usr/src/OPENSOLARIS.LICENSE
 * or http://www.opensolaris.org/os/licensing.
 * See the License for the specific language governing permissions
 * and limitations under the License.
 *
 * When distributing Covered Code, include this CDDL HEADER in each
 * file and include the License file at usr/src/OPENSOLARIS.LICENSE.
 * If applicable, add the following below this CDDL HEADER, with the
 * fields enclosed by brackets "[]" replaced with your own identifying
 * information: Portions Copyright [yyyy] [name of copyright owner]
 *
 * CDDL HEADER END
 */
/*	Copyright (c) 1988 AT&T	*/
/*	  All Rights Reserved	*/


/*
 * Copyright 2014 Garrett D'Amore <garrett@damore.org>
 *
 * Copyright 2007 Sun Microsystems, Inc.  All rights reserved.
 * Use is subject to license terms.
 */
/*
 * Copyright 2010 Nexenta Systems, Inc.  Al rights reserved.
 * Copyright 2016 Joyent, Inc.
 */

#ifndef _TIME_H
#define	_TIME_H

#include <sys/feature_tests.h>
#include <iso/time_iso.h>
/*
 * C11 requires sys/time_impl.h for the definition of the struct timespec.
 */
#if (!defined(_STRICT_STDC) && !defined(__XOPEN_OR_POSIX)) || \
	(_POSIX_C_SOURCE > 2) || defined(__EXTENSIONS__) || defined(_STDC_C11)
#include <sys/types.h>
#include <sys/time_impl.h>
#endif /* (!defined(_STRICT_STDC) && !defined(__XOPEN_OR_POSIX)) ... */

/*
 * Allow global visibility for symbols defined in
 * C++ "std" namespace in <iso/time_iso.h>.
 */
#if __cplusplus >= 199711L
using std::size_t;
using std::clock_t;
using std::time_t;
using std::tm;
using std::asctime;
using std::clock;
using std::ctime;
using std::difftime;
using std::gmtime;
using std::localtime;
using std::mktime;
using std::time;
using std::strftime;
#endif

#ifdef	__cplusplus
extern "C" {
#endif

#ifndef _CLOCKID_T
#define	_CLOCKID_T
typedef int	clockid_t;
#endif

#ifndef _TIMER_T
#define	_TIMER_T
typedef int	timer_t;
#endif

extern char *asctime_r(const struct tm *_RESTRICT_KYWD, char *_RESTRICT_KYWD);
extern char *ctime_r(const time_t *, char *);
extern struct tm *gmtime_r(const time_t *_RESTRICT_KYWD,
			struct tm *_RESTRICT_KYWD);
extern struct tm *localtime_r(const time_t *_RESTRICT_KYWD,
			struct tm *_RESTRICT_KYWD);

#if (!defined(_STRICT_STDC) && !defined(__XOPEN_OR_POSIX)) || \
	defined(_XPG4) || defined(__EXTENSIONS__)

#ifdef _STRPTIME_DONTZERO
#ifdef __PRAGMA_REDEFINE_EXTNAME
#pragma	redefine_extname strptime __strptime_dontzero
#else	/* __PRAGMA_REDEFINE_EXTNAME */
extern char *__strptime_dontzero(const char *, const char *, struct tm *);
#define	strptime	__strptime_dontzero
#endif	/* __PRAGMA_REDEFINE_EXTNAME */
#endif	/* _STRPTIME_DONTZERO */

extern char *strptime(const char *_RESTRICT_KYWD, const char *_RESTRICT_KYWD,
		struct tm *_RESTRICT_KYWD);

#endif /* (!defined(_STRICT_STDC) && !defined(__XOPEN_OR_POSIX))... */

#if (!defined(_STRICT_STDC) && !defined(__XOPEN_OR_POSIX)) || \
	(_POSIX_C_SOURCE > 2) || defined(__EXTENSIONS__)
/*
 * Neither X/Open nor POSIX allow the inclusion of <signal.h> for the
 * definition of the sigevent structure.  Both require the inclusion
 * of <signal.h> and <time.h> when using the timer_create() function.
 * However, X/Open also specifies that the sigevent structure be defined
 * in <time.h> as described in the header <signal.h>.  This prevents
 * compiler warnings for applications that only include <time.h> and not
 * also <signal.h>.  The sigval union and the sigevent structure is
 * therefore defined both here and in <sys/siginfo.h> which gets included
 * via inclusion of <signal.h>.
 */
#ifndef	_SIGVAL
#define	_SIGVAL
union sigval {
	int	sival_int;	/* integer value */
	void	*sival_ptr;	/* pointer value */
};
#endif	/* _SIGVAL */

#ifndef	_SIGEVENT
#define	_SIGEVENT
struct sigevent {
	int		sigev_notify;	/* notification mode */
	int		sigev_signo;	/* signal number */
	union sigval	sigev_value;	/* signal value */
	void		(*sigev_notify_function)(union sigval);
	pthread_attr_t	*sigev_notify_attributes;
	int		__sigev_pad2;
};
#endif	/* _SIGEVENT */

extern int clock_getres(clockid_t, struct timespec *);
extern int clock_gettime(clockid_t, struct timespec *);
extern int clock_settime(clockid_t, const struct timespec *);
extern int timer_create(clockid_t, struct sigevent *_RESTRICT_KYWD,
		timer_t *_RESTRICT_KYWD);
extern int timer_delete(timer_t);
extern int timer_getoverrun(timer_t);
extern int timer_gettime(timer_t, struct itimerspec *);
extern int timer_settime(timer_t, int, const struct itimerspec *_RESTRICT_KYWD,
		struct itimerspec *_RESTRICT_KYWD);

extern int nanosleep(const struct timespec *, struct timespec *);
extern int clock_nanosleep(clockid_t, int,
	const struct timespec *, struct timespec *);

#endif /* (!defined(_STRICT_STDC) && !defined(__XOPEN_OR_POSIX))... */

#if !defined(_STRICT_STDC) || defined(__XOPEN_OR_POSIX) || \
	defined(__EXTENSIONS__)

extern void tzset(void);
extern char *tzname[2];

/* CLK_TCK marked as LEGACY in SUSv2 and removed in SUSv3 */
#if !defined(_XPG6) || defined(__EXTENSIONS__)
#ifndef CLK_TCK
extern long _sysconf(int);	/* System Private interface to sysconf() */
#define	CLK_TCK	((clock_t)_sysconf(3))	/* clock ticks per second */
				/* 3 is _SC_CLK_TCK */
#endif
#endif /* !defined(_XPG6) || defined(__EXTENSIONS__) */

#if (!defined(_STRICT_STDC) && !defined(_POSIX_C_SOURCE)) || \
	defined(_XOPEN_SOURCE) || defined(__EXTENSIONS__)
extern long timezone;
extern int daylight;
#endif

#endif /* !defined(_STRICT_STDC) || defined(__XOPEN_OR_POSIX)... */

#if (!defined(_STRICT_STDC) && !defined(__XOPEN_OR_POSIX)) || \
	defined(__EXTENSIONS__)
extern time_t timegm(struct tm *);
extern int cftime(char *, char *, const time_t *);
extern int ascftime(char *, const char *, const struct tm *);
extern long altzone;
#endif

#if (!defined(_STRICT_STDC) && !defined(__XOPEN_OR_POSIX)) || \
	defined(_XPG4_2) || defined(__EXTENSIONS__)
extern struct tm *getdate(const char *);
#undef getdate_err
#define	getdate_err *(int *)_getdate_err_addr()
extern int *_getdate_err_addr(void);
#endif /* (!defined(_STRICT_STDC) && !defined(__XOPEN_OR_POSIX))... */

#if defined(_XPG7) || !defined(_STRICT_SYMBOLS)

#ifndef	_LOCALE_T
#define	_LOCALE_T
typedef struct _locale *locale_t;
#endif

extern size_t strftime_l(char *_RESTRICT_KYWD, size_t,
	const char *_RESTRICT_KYWD, const struct tm *_RESTRICT_KYWD, locale_t);

#endif /* defined(_XPG7) || !defined(_STRICT_SYMBOLS) */

#if !defined(_STRICT_SYMBOLS) || defined(_STDC_C11)

/*
 * Note, the C11 standard requires that all the various base values that are
 * passed into timespec_get() be non-zero. Hence why TIME_UTC starts at one.
 */
#define	TIME_UTC	0x1		/* timespec_get base */

extern int timespec_get(struct timespec *, int);
#endif

#ifdef	__cplusplus
}
#endif

#endif	/* _TIME_H */
