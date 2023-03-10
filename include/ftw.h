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


#ifndef	_FTW_H
#define	_FTW_H

#include <sys/feature_tests.h>

#include <sys/types.h>
#include <sys/stat.h>

#ifdef	__cplusplus
extern "C" {
#endif

/*
 *	Codes for the third argument to the user-supplied function.
 */

#define	FTW_F	0	/* file */
#define	FTW_D	1	/* directory */
#define	FTW_DNR	2	/* directory without read permission */
#define	FTW_NS	3	/* unknown type, stat failed */
#define	FTW_SL	4	/* symbolic link */
#define	FTW_DP	6	/* directory */
#define	FTW_SLN	7	/* symbolic link that points to nonexistent file */
#define	FTW_DL	8	/* private interface for find utility */

/*
 *	Codes for the fourth argument to nftw.  You can specify the
 *	union of these flags.
 */

#define	FTW_PHYS	01  /* use lstat instead of stat */
#define	FTW_MOUNT	02  /* do not cross a mount point */
#define	FTW_CHDIR	04  /* chdir to each directory before reading */
#define	FTW_DEPTH	010 /* call descendents before calling the parent */
#define	FTW_ANYERR	020 /* return FTW_NS on any stat failure */
#define	FTW_HOPTION	040 /* private interface for find utility */
#define	FTW_NOLOOP	0100 /* private interface for find utility */

#if defined(__EXTENSIONS__) || !defined(_XOPEN_SOURCE) || defined(_XPG4_2)
struct FTW
{
	int	__quit;
	int	base;
	int	level;
};
#endif /* defined(__EXTENSIONS__) || !defined(_XOPEN_SOURCE) ... */

/*
 * legal values for quit
 */

#define	FTW_SKD		1
#define	FTW_FOLLOW	2
#define	FTW_PRUNE	4

extern int ftw(const char *,
	int (*)(const char *, const struct stat *, int), int);
extern int _xftw(int, const char *,
	int (*)(const char *, const struct stat *, int), int);
#if defined(__EXTENSIONS__) || !defined(_XOPEN_SOURCE) || defined(_XPG4_2)
extern int nftw(const char *,
	int (*)(const char *, const struct stat *, int, struct FTW *),
	int, int);
#endif /* defined(__EXTENSIONS__) || !defined(_XOPEN_SOURCE) ... */

#define	_XFTWVER	2	/* version of file tree walk */

#ifdef	__cplusplus
}
#endif

#endif	/* _FTW_H */
