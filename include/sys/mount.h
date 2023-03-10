/*
 * CDDL HEADER START
 *
 * The contents of this file are subject to the terms of the
 * Common Development and Distribution License, Version 1.0 only
 * (the "License").  You may not use this file except in compliance
 * with the License.
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
 * Copyright (c) 1996, 1999 by Sun Microsystems, Inc.
 * All rights reserved.
 */

#ifndef _SYS_MOUNT_H
#define	_SYS_MOUNT_H

#ifdef	__cplusplus
extern "C" {
#endif

/*
 * Flag bits passed to mount(2).
 */
#define	MS_RDONLY	0x0001	/* Read-only */
#define	MS_FSS		0x0002	/* Old (4-argument) mount (compatibility) */
#define	MS_DATA		0x0004	/* 6-argument mount */
#define	MS_NOSUID	0x0010	/* Setuid programs disallowed */
#define	MS_REMOUNT	0x0020	/* Remount */
#define	MS_NOTRUNC	0x0040	/* Return ENAMETOOLONG for long filenames */
#define	MS_OVERLAY	0x0080	/* Allow overlay mounts */
#define	MS_OPTIONSTR	0x0100	/* Data is a an in/out option string */
#define	MS_FORCE	0x0400	/* Forced unmount */
#define	MS_NOMNTTAB	0x0800	/* Don't show mount in mnttab */
/*
 * Additional flag bits that domount() is prepared to interpret, but that
 * can't be passed through mount(2).
 */
#define	MS_SYSSPACE	0x0008	/* Mounta already in kernel space */
#define	MS_NOSPLICE	0x1000	/* Don't splice fs instance into name space */
#define	MS_NOCHECK	0x2000	/* Clustering: suppress mount busy checks */
/*
 * Mask to sift out flag bits allowable from mount(2).
 */
#define	MS_MASK	\
	(MS_RDONLY|MS_FSS|MS_DATA|MS_NOSUID|MS_REMOUNT|MS_NOTRUNC|MS_OVERLAY|\
	    MS_OPTIONSTR|MS_NOMNTTAB)

/*
 * Mask to sift out flag bits allowable from umount2(2).
 */

#define	MS_UMOUNT_MASK	(MS_FORCE)

/*
 * Maximum option string length accepted or returned by mount(2).
 */
#define	MAX_MNTOPT_STR	1024	/* max length of mount options string */

#if !defined(_KERNEL)
int mount(const char *, const char *, int, ...);
int umount(const char *);
int umount2(const char *, int);
#endif

#ifdef	__cplusplus
}
#endif

#endif	/* _SYS_MOUNT_H */
