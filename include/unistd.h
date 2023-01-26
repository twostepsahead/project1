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

/*
 * Copyright 2014 PALO, Richard.
 * Copyright 2014 Garrett D'Amore <garrett@damore.org>
 * Copyright (c) 2013 Gary Mills
 *
 * Copyright (c) 1989, 2010, Oracle and/or its affiliates. All rights reserved.
 */

/*	Copyright (c) 1988 AT&T	*/
/*	  All Rights Reserved	*/

/* Copyright (c) 2013, OmniTI Computer Consulting, Inc. All rights reserved. */

#ifndef _UNISTD_H
#define	_UNISTD_H

#include <sys/feature_tests.h>

#include <sys/null.h>
#include <sys/types.h>
#include <sys/unistd.h>

#ifdef	__cplusplus
extern "C" {
#endif

/* Symbolic constants for the "access" routine: */
#define	R_OK	4	/* Test for Read permission */
#define	W_OK	2	/* Test for Write permission */
#define	X_OK	1	/* Test for eXecute permission */
#define	F_OK	0	/* Test for existence of File */

#if !defined(__XOPEN_OR_POSIX) || defined(_XPG4_2) || defined(__EXTENSIONS__)
#define	F_ULOCK	0	/* Unlock a previously locked region */
#define	F_LOCK	1	/* Lock a region for exclusive use */
#define	F_TLOCK	2	/* Test and lock a region for exclusive use */
#define	F_TEST	3	/* Test a region for other processes locks */
#endif /* !defined(__XOPEN_OR_POSIX) || defined(_XPG4_2)... */

/* Symbolic constants for the "lseek" routine: */

#ifndef	SEEK_SET
#define	SEEK_SET	0	/* Set file pointer to "offset" */
#endif

#ifndef	SEEK_CUR
#define	SEEK_CUR	1	/* Set file pointer to current plus "offset" */
#endif

#ifndef	SEEK_END
#define	SEEK_END	2	/* Set file pointer to EOF plus "offset" */
#endif

#if !defined(__XOPEN_OR_POSIX) || defined(__EXTENSIONS__)
#ifndef	SEEK_DATA
#define	SEEK_DATA	3	/* Set file pointer to next data past offset */
#endif

#ifndef	SEEK_HOLE
#define	SEEK_HOLE	4	/* Set file pointer to next hole past offset */
#endif
#endif /* !defined(__XOPEN_OR_POSIX) || defined(__EXTENSIONS__) */

#if !defined(__XOPEN_OR_POSIX) || defined(__EXTENSIONS__)
/* Path names: */
#define	GF_PATH	"/etc/group"	/* Path name of the "group" file */
#define	PF_PATH	"/etc/passwd"	/* Path name of the "passwd" file */
#endif /* !defined(__XOPEN_OR_POSIX) || defined(__EXTENSIONS__) */

/*
 * compile-time symbolic constants,
 * Support does not mean the feature is enabled.
 * Use pathconf/sysconf to obtain actual configuration value.
 */

/* Values unchanged in UNIX 03 */
#define	_POSIX_ASYNC_IO			1
#define	_POSIX_JOB_CONTROL		1
#define	_POSIX_SAVED_IDS		1
#define	_POSIX_SYNC_IO			1

/*
 * POSIX.1b compile-time symbolic constants.
 */
#if defined(_XPG6)
#define	_POSIX_ASYNCHRONOUS_IO		200112L
#define	_POSIX_FSYNC			200112L
#define	_POSIX_MAPPED_FILES		200112L
#define	_POSIX_MEMLOCK			200112L
#define	_POSIX_MEMLOCK_RANGE		200112L
#define	_POSIX_MEMORY_PROTECTION	200112L
#define	_POSIX_MESSAGE_PASSING		200112L
#define	_POSIX_PRIORITY_SCHEDULING	200112L
#define	_POSIX_REALTIME_SIGNALS		200112L
#define	_POSIX_SEMAPHORES		200112L
#define	_POSIX_SHARED_MEMORY_OBJECTS	200112L
#define	_POSIX_SYNCHRONIZED_IO		200112L
#else
#define	_POSIX_ASYNCHRONOUS_IO		1
#define	_POSIX_FSYNC			1
#define	_POSIX_MAPPED_FILES		1
#define	_POSIX_MEMLOCK			1
#define	_POSIX_MEMLOCK_RANGE		1
#define	_POSIX_MEMORY_PROTECTION	1
#define	_POSIX_MESSAGE_PASSING		1
#define	_POSIX_PRIORITY_SCHEDULING	1
#define	_POSIX_REALTIME_SIGNALS		1
#define	_POSIX_SEMAPHORES		1
#define	_POSIX_SHARED_MEMORY_OBJECTS	1
#define	_POSIX_SYNCHRONIZED_IO		1
#endif

/*
 * POSIX.1c compile-time symbolic constants.
 */
#if defined(_XPG6)
#define	_POSIX_THREAD_SAFE_FUNCTIONS		200112L
#define	_POSIX_THREADS				200112L
#define	_POSIX_THREAD_ATTR_STACKADDR		200112L
#define	_POSIX_THREAD_ATTR_STACKSIZE		200112L
#define	_POSIX_THREAD_PROCESS_SHARED		200112L
#define	_POSIX_THREAD_PRIORITY_SCHEDULING	200112L
#define	_POSIX_TIMERS				200112L
#else
#define	_POSIX_THREAD_SAFE_FUNCTIONS		1
#define	_POSIX_THREADS				1
#define	_POSIX_THREAD_ATTR_STACKADDR		1
#define	_POSIX_THREAD_ATTR_STACKSIZE		1
#define	_POSIX_THREAD_PROCESS_SHARED		1
#define	_POSIX_THREAD_PRIORITY_SCHEDULING	1
#define	_POSIX_TIMERS				1
#endif

/* New in UNIX 03 */
#define	_POSIX_ADVISORY_INFO			200112L
#define	_POSIX_BARRIERS				200112L
#define	_POSIX_CLOCK_SELECTION			200112L
#define	_POSIX_IPV6				200112L
#define	_POSIX_MONOTONIC_CLOCK			200112L
#define	_POSIX_RAW_SOCKETS			200112L
#define	_POSIX_READER_WRITER_LOCKS		200112L
#define	_POSIX_SPAWN				200112L
#define	_POSIX_SPIN_LOCKS			200112L
#define	_POSIX_TIMEOUTS				200112L

/*
 * Support for the POSIX.1 mutex protocol attribute. For realtime applications
 * which need mutexes to support priority inheritance/ceiling.
 */
#if defined(_XPG6)
#define	_POSIX_THREAD_PRIO_INHERIT		200112L
#define	_POSIX_THREAD_PRIO_PROTECT		200112L
#else
#define	_POSIX_THREAD_PRIO_INHERIT		1
#define	_POSIX_THREAD_PRIO_PROTECT		1
#endif

#ifndef _POSIX_VDISABLE
#define	_POSIX_VDISABLE		0
#endif

#define	STDIN_FILENO	0
#define	STDOUT_FILENO	1
#define	STDERR_FILENO	2

extern int access(const char *, int);
#if !defined(__XOPEN_OR_POSIX) || defined(__EXTENSIONS__)
extern int acct(const char *);
#endif /* !defined(__XOPEN_OR_POSIX) || defined(__EXTENSIONS__) */
extern unsigned alarm(unsigned);
/* Marked as LEGACY in SUSv2 and removed in SUSv3 */
#if !defined(__XOPEN_OR_POSIX) || (defined(_XPG4_2) && !defined(_XPG6)) || \
	defined(__EXTENSIONS__)
extern int brk(void *);
#endif /* !defined(__XOPEN_OR_POSIX) || (defined(_XPG4_2)... */
extern int chdir(const char *);
extern int chown(const char *, uid_t, gid_t);
/* Marked as LEGACY in SUSv2 and removed in SUSv3 */
#if !defined(_POSIX_C_SOURCE) || (defined(_XOPEN_SOURCE) && \
	!defined(_XPG6)) || defined(__EXTENSIONS__)
extern int chroot(const char *);
#endif /* !defined(_POSIX_C_SOURCE) || defined(_XOPEN_SOURCE))... */
extern int close(int);
#if defined(_XPG4) || defined(__EXTENSIONS__)
extern size_t confstr(int, char *, size_t);
extern char *crypt(const char *, const char *);
#endif /* defined(_XPG4) || defined(__EXTENSIONS__) */
#if !defined(_POSIX_C_SOURCE) || defined(_XOPEN_SOURCE) || \
	defined(__EXTENSIONS__)
extern char *ctermid(char *);
#endif /* (!defined(_POSIX_C_SOURCE) ... */
extern char *ctermid_r(char *);
/* Marked as LEGACY in SUSv2 and removed in SUSv3 */
#if !defined(_XPG6) || defined(__EXTENSIONS__)
extern char *cuserid(char *);
#endif
extern int dup(int);
extern int dup2(int, int);
extern int dup3(int, int, int);
#if defined(_XPG4) || defined(__EXTENSIONS__)
extern void encrypt(char *, int);
#endif /* defined(XPG4) || defined(__EXTENSIONS__) */
#if !defined(__XOPEN_OR_POSIX) || defined(__EXTENSIONS__)
extern void endusershell(void);
#endif /* !defined(__XOPEN_OR_POSIX) || defined(__EXTENSIONS__) */
extern int execl(const char *, const char *, ...);
extern int execle(const char *, const char *, ...);
extern int execlp(const char *, const char *, ...);
extern int execv(const char *, char *const *);
extern int execve(const char *, char *const *, char *const *);
extern int execvp(const char *, char *const *);
#if defined(_GNU_SOURCE) || defined(__EXTENSIONS__)
extern int execvpe(const char *, char *const *, char *const *);
#endif
extern void _exit(int)
	__NORETURN;
/*
 * The following fattach prototype is duplicated in <stropts.h>. The
 * duplication is necessitated by XPG4.2 which requires the prototype
 * be defined in <stropts.h>.
 */
#if !defined(__XOPEN_OR_POSIX) || defined(__EXTENSIONS__)
extern int fattach(int, const char *);
#endif /* !defined(__XOPEN_OR_POSIX) || defined(__EXTENSIONS__) */
#if !defined(__XOPEN_OR_POSIX) || defined(_XPG4_2) || defined(__EXTENSIONS__)
extern int fchdir(int);
extern int fchown(int, uid_t, gid_t);
#endif /* !defined(__XOPEN_OR_POSIX) || defined(_XPG4_2)... */
#if !defined(__XOPEN_OR_POSIX) || defined(__EXTENSIONS__)
extern int fchroot(int);
#endif /* !defined(__XOPEN_OR_POSIX) || defined(__EXTENSIONS__) */
#if !defined(__XOPEN_OR_POSIX) || (_POSIX_C_SOURCE > 2) || \
	defined(__EXTENSIONS__)
extern int fdatasync(int);
#endif /* !defined(__XOPEN_OR_POSIX) || (_POSIX_C_SOURCE > 2)... */
/*
 * The following fdetach prototype is duplicated in <stropts.h>. The
 * duplication is necessitated by XPG4.2 which requires the prototype
 * be defined in <stropts.h>.
 */
#if !defined(__XOPEN_OR_POSIX) || defined(__EXTENSIONS__)
extern int fdetach(const char *);
#endif /* !defined(__XOPEN_OR_POSIX)... */
extern pid_t fork(void);
#if !defined(__XOPEN_OR_POSIX) || defined(__EXTENSIONS__)
extern pid_t fork1(void);
extern pid_t forkall(void);
#endif /* !defined(__XOPEN_OR_POSIX)... */
extern long fpathconf(int, int);
#if !defined(_POSIX_C_SOURCE) || (_POSIX_C_SOURCE > 2) || \
	defined(__EXTENSIONS__)
extern int fsync(int);
#endif /* !defined(_POSIX_C_SOURCE) || (_POSIX_C_SOURCE > 2)... */
extern int ftruncate(int, off_t);
extern char *getcwd(char *, size_t);
#if !defined(__XOPEN_OR_POSIX) || (defined(_XPG4_2) && !defined(_XPG6)) || \
	defined(__EXTENSIONS__)
extern int getdtablesize(void);
#endif
extern gid_t getegid(void);
extern uid_t geteuid(void);
extern gid_t getgid(void);
extern int getgroups(int, gid_t *);
#if !defined(__XOPEN_OR_POSIX) || defined(_XPG4_2) || defined(__EXTENSIONS__)
extern long gethostid(void);
#endif
extern int gethostname(char *, size_t);

#ifndef	__GETLOGIN_DEFINED	/* Avoid duplicate in stdlib.h */
extern char *getlogin(void);
#endif	/* __GETLOGIN_DEFINED */

#if defined(_XPG4) || defined(__EXTENSIONS__)
extern int  getopt(int, char *const *, const char *);
extern char *optarg;
extern int  opterr, optind, optopt;
/* Marked as LEGACY in SUSv2 and removed in SUSv3 */
#if !defined(_XPG6) || defined(__EXTENSIONS__)
extern char *getpass(const char *);
#endif
#endif /* defined(_XPG4) || defined(__EXTENSIONS__) */
#if !defined(__XOPEN_OR_POSIX) || defined(_XPG4_2) || defined(__EXTENSIONS__)
/* Marked as LEGACY in SUSv2 and removed in SUSv3 */
#if !defined(_XPG6) || defined(__EXTENSIONS__)
extern int getpagesize(void);
#endif
extern pid_t getpgid(pid_t);
#endif /* !defined(__XOPEN_OR_POSIX) || defined(_XPG4_2)... */
extern pid_t getpid(void);
extern pid_t getppid(void);
extern pid_t getpgrp(void);

#if !defined(__XOPEN_OR_POSIX) || defined(__EXTENSIONS__)
char *gettxt(const char *, const char *);
#endif /* !defined(__XOPEN_OR_POSIX) || defined(__EXTENSIONS__) */
#if !defined(__XOPEN_OR_POSIX) || defined(_XPG4_2) || defined(__EXTENSIONS__)
extern pid_t getsid(pid_t);
#endif /* !defined(__XOPEN_OR_POSIX) || defined(_XPG4_2)... */
extern uid_t getuid(void);
#if !defined(__XOPEN_OR_POSIX) || defined(__EXTENSIONS__)
extern char *getusershell(void);
#endif /* !defined(__XOPEN_OR_POSIX) || defined(__EXTENSIONS__) */
/*
 * The following ioctl prototype is duplicated in <stropts.h>. The
 * duplication is necessitated by XPG4.2 which requires the prototype
 * be defined in <stropts.h>.
 */
#if !defined(__XOPEN_OR_POSIX) || defined(__EXTENSIONS__)
extern int ioctl(int, int, ...);
extern int isaexec(const char *, char *const *, char *const *);
extern int issetugid(void);
#endif
extern int isatty(int);
#if !defined(__XOPEN_OR_POSIX) || defined(_XPG4_2) || defined(__EXTENSIONS__)
extern int lchown(const char *, uid_t, gid_t);
#endif
extern int link(const char *, const char *);
#if !defined(__XOPEN_OR_POSIX) || defined(__EXTENSIONS__)
extern offset_t llseek(int, offset_t, int);
#endif
extern int lockf(int, int, off_t);
extern off_t lseek(int, off_t, int);
#if !defined(_POSIX_C_SOURCE) || defined(_XOPEN_SOURCE) || \
	defined(__EXTENSIONS__)
extern int nice(int);
#endif /* !defined(_POSIX_C_SOURCE) || defined(_XOPEN_SOURCE)... */
#if !defined(__XOPEN_OR_POSIX) || defined(__EXTENSIONS__)
extern int mincore(caddr_t, size_t, char *);
#endif
extern long pathconf(const char *, int);
extern int pause(void);
extern int pipe(int *);
extern int pipe2(int *, int);
extern ssize_t pread(int, void *, size_t, off_t);
#if !defined(__XOPEN_OR_POSIX) || defined(__EXTENSIONS__)
extern void profil(unsigned short *, size_t, unsigned long, unsigned int);
#endif
/*
 * pthread_atfork() is also declared in <pthread.h> as per SUSv3. The
 * declarations are identical. A change to either one may also require
 * appropriate namespace updates in order to avoid redeclaration
 * warnings in the case where both prototypes are exposed via inclusion
 * of both <pthread.h> and <unistd.h>.
 */
#if !defined(__XOPEN_OR_POSIX) || \
	((_POSIX_C_SOURCE > 2) && !defined(_XPG6)) || \
	defined(__EXTENSIONS__)
extern int pthread_atfork(void (*) (void), void (*) (void), void (*) (void));
#endif /* !defined(__XOPEN_OR_POSIX) || ((_POSIX_C_SOURCE > 2) ... */
#if !defined(_LP64) && \
	(!defined(__XOPEN_OR_POSIX) || defined(__EXTENSIONS__))
extern int ptrace(int, pid_t, int, int);
#endif
extern ssize_t pwrite(int, const void *, size_t, off_t);
extern ssize_t read(int, void *, size_t);
#if !defined(__XOPEN_OR_POSIX) || \
	defined(_XPG4_2) || defined(__EXTENSIONS__)
extern ssize_t readlink(const char *_RESTRICT_KYWD, char *_RESTRICT_KYWD,
	size_t);
#endif
#if (!defined(__XOPEN_OR_POSIX) || (defined(_XPG3) && !defined(_XPG4))) || \
	defined(__EXTENSIONS__)
#if __cplusplus >= 199711L
namespace std {
#endif
extern int rename(const char *, const char *);
#if __cplusplus >= 199711L
} /* end of namespace std */

using std::rename;
#endif /* __cplusplus >= 199711L */
#endif /* (!defined(__XOPEN_OR_POSIX) || (defined(_XPG3)... */
#if !defined(__XOPEN_OR_POSIX) || defined(__EXTENSIONS__)
extern int resolvepath(const char *, char *, size_t);
#endif /* !defined(__XOPEN_OR_POSIX)|| defined(__EXTENSIONS__) */
extern int rmdir(const char *);

#if !defined(__XOPEN_OR_POSIX) || (defined(_XPG4_2) && !defined(_XPG6)) || \
	defined(__EXTENSIONS__)
extern void *sbrk(intptr_t);
#endif /* !defined(__XOPEN_OR_POSIX) || (defined(_XPG4_2)... */
#if !defined(__XOPEN_OR_POSIX) || defined(_XPG6) || defined(__EXTENSIONS__)
extern int setegid(gid_t);
extern int seteuid(uid_t);
#endif /* !defined(__XOPEN_OR_POSIX) || defined(_XPG6) ... */
extern int setgid(gid_t);
#if !defined(__XOPEN_OR_POSIX) || defined(__EXTENSIONS__)
extern int setgroups(int, const gid_t *);
extern int sethostname(char *, int);
#endif /* !defined(__XOPEN_OR_POSIX) || defined(__EXTENSIONS__) */
extern int setpgid(pid_t, pid_t);
#if !defined(__XOPEN_OR_POSIX) || defined(_XPG4_2) || defined(__EXTENSIONS__)
extern pid_t setpgrp(void);
extern int setregid(gid_t, gid_t);
extern int setreuid(uid_t, uid_t);
#endif /* !defined(__XOPEN_OR_POSIX) || defined(_XPG4_2)... */
extern pid_t setsid(void);
extern int setuid(uid_t);
#if !defined(__XOPEN_OR_POSIX) || defined(__EXTENSIONS__)
extern void setusershell(void);
#endif /* !defined(__XOPEN_OR_POSIX)|| defined(__EXTENSIONS__) */
extern unsigned sleep(unsigned);
#if !defined(__XOPEN_OR_POSIX) || defined(__EXTENSIONS__)
extern int stime(const time_t *);
#endif /* !defined(__XOPEN_OR_POSIX) || defined(__EXTENSIONS__) */
#if defined(_XPG4)
/* __EXTENSIONS__ makes the SVID Third Edition prototype in stdlib.h visible */
extern void swab(const void *_RESTRICT_KYWD, void *_RESTRICT_KYWD, ssize_t);
#endif /* defined(_XPG4) */
#if !defined(__XOPEN_OR_POSIX) || defined(_XPG4_2) || defined(__EXTENSIONS__)
extern int symlink(const char *, const char *);
extern void sync(void);
#endif /* !defined(__XOPEN_OR_POSIX) || defined(_XPG4_2) */
#if defined(_XPG5) && !defined(_XPG6)
#ifdef __PRAGMA_REDEFINE_EXTNAME
#pragma redefine_extname sysconf __sysconf_xpg5
#else /* __PRAGMA_REDEFINE_EXTNAME */
#define	sysconf __sysconf_xpg5
#endif  /* __PRAGMA_REDEFINE_EXTNAME */
#endif /* defined(_XPG5) && !defined(_XPG6) */
extern long sysconf(int);
extern pid_t tcgetpgrp(int);
extern int tcsetpgrp(int, pid_t);
#if !defined(__XOPEN_OR_POSIX) || defined(__EXTENSIONS__)
extern off_t tell(int);
#endif /* !defined(__XOPEN_OR_POSIX)... */
#if __XPG_VISIBLE >= 420
extern int truncate(const char *, off_t);
#endif
extern char *ttyname(int);
#if (defined(_XPG4_2) && !defined(_XPG7)) || !defined(_STRICT_SYMBOLS)
extern useconds_t ualarm(useconds_t, useconds_t);
#endif
extern int unlink(const char *);
#if (defined(_XPG4_2) && !defined(_XPG7)) || !defined(_STRICT_SYMBOLS)
extern char *getwd(char *);
extern int usleep(useconds_t);
extern pid_t vfork(void) __RETURNS_TWICE;
#pragma unknown_control_flow(vfork)
#endif
#if !defined(__XOPEN_OR_POSIX) || defined(__EXTENSIONS__)
extern void vhangup(void);
#endif /* !defined(__XOPEN_OR_POSIX) || defined(__EXTENSIONS__) */
extern ssize_t write(int, const void *, size_t);
#if !defined(__XOPEN_OR_POSIX) || defined(__EXTENSIONS__)
extern void yield(void);
#endif /* !defined(__XOPEN_OR_POSIX) || defined(__EXTENSIONS__) */

#if !defined(__XOPEN_OR_POSIX) || defined(_ATFILE_SOURCE) || \
	defined(__EXTENSIONS__)
	/* || defined(_XPG7) */
extern int faccessat(int, const char *, int, int);
extern int fchownat(int, const char *, uid_t, gid_t, int);
extern int linkat(int, const char *, int, const char *, int);
extern ssize_t readlinkat(int, const char *_RESTRICT_KYWD,
	char *_RESTRICT_KYWD, size_t);
extern int renameat(int, const char *, int, const char *);
extern int symlinkat(const char *, int, const char *);
extern int unlinkat(int, const char *, int);
#endif	/* !defined(__XOPEN_OR_POSIX) || defined(_ATFILE_SOURCE)... */
#if !defined(__XOPEN_OR_POSIX) || defined(__EXTENSIONS__)
extern int get_nprocs(void);
extern int get_nprocs_conf(void);
#endif /* !defined(__XOPEN_OR_POSIX) || defined(__EXTENSIONS__) */

extern int getlogin_r(char *, size_t);
extern int ttyname_r(int, char *, size_t);

#if !defined(_STRICT_SYMBOLS)
extern int getentropy(void *, size_t);
#endif	/* !_STRICT_SYMBOLS */

#if __UNLEASHED_VISIBLE
extern mode_t getmode(const void *, mode_t);
extern void *setmode(const char *);
#endif

#ifdef	__cplusplus
}
#endif

#endif /* _UNISTD_H */
