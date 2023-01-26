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
/*	Copyright (c) 1984, 1986, 1987, 1988, 1989 AT&T	*/
/*	  All Rights Reserved	*/


/*
 * Copyright 2014 Garrett D'Amore <garrett@damore.org>
 *
 * Copyright 2009 Sun Microsystems, Inc.  All rights reserved.
 * Use is subject to license terms.
 */

#ifndef _SYS_DIRENT_H
#define	_SYS_DIRENT_H

#include <sys/feature_tests.h>

#ifdef	__cplusplus
extern "C" {
#endif

/*
 * File-system independent directory entry.
 */
typedef struct dirent {
	ino_t		d_ino;		/* "inode number" of entry */
	off_t		d_off;		/* offset of disk directory entry */
	unsigned short	d_reclen;	/* length of this record */
	char		d_name[1];	/* name of file */
} dirent_t;

#if !defined(__XOPEN_OR_POSIX) || defined(__EXTENSIONS__)
#if defined(_KERNEL)
#define	DIRENT_RECLEN(namelen)	\
	((offsetof(dirent_t, d_name[0]) + 1 + (namelen) + 7) & ~ 7)
#define	DIRENT_NAMELEN(reclen)	\
	((reclen) - (offsetof(dirent_t, d_name[0])))
#endif

/*
 * This is the maximum number of bytes that getdents(2) will store in
 * user-supplied dirent buffers.
 */
#define	MAXGETDENTS_SIZE	(64 * 1024)

#if !defined(_KERNEL)

extern int getdents(int, struct dirent *, size_t);

#endif /* !defined(_KERNEL) */
#endif /* !defined(__XOPEN_OR_POSIX) || defined(__EXTENSIONS__) */

#ifdef	__cplusplus
}
#endif

#endif	/* _SYS_DIRENT_H */
