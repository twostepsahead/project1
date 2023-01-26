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
 * Copyright 2014 Garrett D'Amore <garrett@damore.org>
 *
 * Copyright 2007 Sun Microsystems, Inc.  All rights reserved.
 * Use is subject to license terms.
 */

/*	Copyright (c) 1988 AT&T	*/
/*	  All Rights Reserved	*/

#ifndef _DIRENT_H
#define	_DIRENT_H

#include <sys/feature_tests.h>

#include <sys/types.h>
#include <sys/dirent.h>

#ifdef	__cplusplus
extern "C" {
#endif

#if defined(__EXTENSIONS__) || !defined(__XOPEN_OR_POSIX)

#define	MAXNAMLEN	512		/* maximum filename length */
#define	DIRBUF		8192		/* buffer size for fs-indep. dirs */

#endif /* defined(__EXTENSIONS__) || !defined(__XOPEN_OR_POSIX) */

#if !defined(__XOPEN_OR_POSIX)

typedef struct {
	int	dd_fd;		/* file descriptor */
	int	dd_loc;		/* offset in block */
	int	dd_size;	/* amount of valid data */
	char	*dd_buf;	/* directory block */
} DIR;				/* stream data from opendir() */


#else

typedef struct {
	int	d_fd;		/* file descriptor */
	int	d_loc;		/* offset in block */
	int	d_size;		/* amount of valid data */
	char	*d_buf;		/* directory block */
} DIR;				/* stream data from opendir() */

#endif /* !defined(__XOPEN_OR_POSIX) */

extern DIR		*opendir(const char *);
#if defined(__EXTENSIONS__) || !defined(__XOPEN_OR_POSIX) || \
	defined(_ATFILE_SOURCE)
extern DIR		*fdopendir(int);
extern int		dirfd(DIR *);
#endif /* defined(__EXTENSIONS__) || !defined(__XOPEN_OR_POSIX) ... */
#if defined(__EXTENSIONS__) || !defined(__XOPEN_OR_POSIX)
extern int		scandir(const char *, struct dirent *(*[]),
				int (*)(const struct dirent *),
				int (*)(const struct dirent **,
					const struct dirent **));
extern int		alphasort(const struct dirent **,
					const struct dirent **);
#endif /* defined(__EXTENSIONS__) || !defined(__XOPEN_OR_POSIX) */
extern struct dirent	*readdir(DIR *);
#if defined(__EXTENSIONS__) || !defined(_POSIX_C_SOURCE) || \
	defined(_XOPEN_SOURCE)
extern long		telldir(DIR *);
extern void		seekdir(DIR *, long);
#endif /* defined(__EXTENSIONS__) || !defined(_POSIX_C_SOURCE) ... */
extern void		rewinddir(DIR *);
extern int		closedir(DIR *);

#if defined(__EXTENSIONS__) || !defined(_POSIX_C_SOURCE) || \
	defined(_XOPEN_SOURCE)
#define	rewinddir(dirp)	seekdir(dirp, 0L)
#endif

extern int readdir_r(DIR *_RESTRICT_KYWD, struct dirent *_RESTRICT_KYWD,
	struct dirent **_RESTRICT_KYWD);

#ifdef	__cplusplus
}
#endif

#endif	/* _DIRENT_H */
