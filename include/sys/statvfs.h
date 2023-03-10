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
 * Copyright 2004 Sun Microsystems, Inc.  All rights reserved.
 * Use is subject to license terms.
 */

#ifndef _SYS_STATVFS_H
#define	_SYS_STATVFS_H

#include <sys/feature_tests.h>
#include <sys/types.h>

#ifdef	__cplusplus
extern "C" {
#endif

/*
 * Structure returned by statvfs(2).
 */

#define	_FSTYPSZ	16
#if !defined(_XPG4_2) || defined(__EXTENSIONS__)
#ifndef FSTYPSZ
#define	FSTYPSZ	_FSTYPSZ
#endif
#endif /* !defined(_XPG4_2) || defined(__EXTENSIONS__) */

typedef struct statvfs {
	unsigned long	f_bsize;	/* fundamental file system block size */
	unsigned long	f_frsize;	/* fragment size */
	fsblkcnt_t	f_blocks;	/* total blocks of f_frsize on fs */
	fsblkcnt_t	f_bfree;	/* total free blocks of f_frsize */
	fsblkcnt_t	f_bavail;	/* free blocks avail to non-superuser */
	fsfilcnt_t	f_files;	/* total file nodes (inodes) */
	fsfilcnt_t	f_ffree;	/* total free file nodes */
	fsfilcnt_t	f_favail;	/* free nodes avail to non-superuser */
	unsigned long	f_fsid;		/* file system id (dev for now) */
	char		f_basetype[_FSTYPSZ];	/* target fs type name, */
						/* null-terminated */
	unsigned long	f_flag;		/* bit-mask of flags */
	unsigned long	f_namemax;	/* maximum file name length */
	char		f_fstr[32];	/* filesystem-specific string */
#if !defined(_LP64)
	unsigned long 	f_filler[16];	/* reserved for future expansion */
#endif
} statvfs_t;

#if defined(_SYSCALL32)

/* Kernel view of user ILP32 statvfs structure */

typedef struct statvfs32 {
	uint32_t	f_bsize;	/* fundamental file system block size */
	uint32_t	f_frsize;	/* fragment size */
	fsblkcnt32_t	f_blocks;	/* total blocks of f_frsize on fs */
	fsblkcnt32_t	f_bfree;	/* total free blocks of f_frsize */
	fsblkcnt32_t	f_bavail;	/* free blocks avail to non-superuser */
	fsfilcnt32_t	f_files;	/* total file nodes (inodes) */
	fsfilcnt32_t	f_ffree;	/* total free file nodes */
	fsfilcnt32_t	f_favail;	/* free nodes avail to non-superuser */
	uint32_t	f_fsid;		/* file system id (dev for now) */
	char		f_basetype[_FSTYPSZ];	/* target fs type name, */
						/* null-terminated */
	uint32_t	f_flag;		/* bit-mask of flags */
	uint32_t	f_namemax;	/* maximum file name length */
	char		f_fstr[32];	/* filesystem-specific string */
	uint32_t	f_filler[16];	/* reserved for future expansion */
} statvfs32_t;

#endif	/* _SYSCALL32 */

/* transitional large file interface version */
typedef struct statvfs64 {
	unsigned long	f_bsize;	/* preferred file system block size */
	unsigned long	f_frsize;	/* fundamental file system block size */
	fsblkcnt64_t	f_blocks;	/* total blocks of f_frsize */
	fsblkcnt64_t	f_bfree;	/* total free blocks of f_frsize */
	fsblkcnt64_t	f_bavail;	/* free blocks avail to non-superuser */
	fsfilcnt64_t	f_files;	/* total # of file nodes (inodes) */
	fsfilcnt64_t	f_ffree;	/* total # of free file nodes */
	fsfilcnt64_t	f_favail;	/* free nodes avail to non-superuser */
	unsigned long	f_fsid;		/* file system id (dev for now) */
	char		f_basetype[_FSTYPSZ];	/* target fs type name, */
						/* null-terminated */
	unsigned long	f_flag;		/* bit-mask of flags */
	unsigned long	f_namemax;	/* maximum file name length */
	char		f_fstr[32];	/* filesystem-specific string */
#if !defined(_LP64)
	unsigned long	f_filler[16];	/* reserved for future expansion */
#endif	/* _LP64 */
} statvfs64_t;

#if defined(_SYSCALL32)

/* Kernel view of user ILP32 statvfs64 structure */

#if _LONG_LONG_ALIGNMENT == 8 && _LONG_LONG_ALIGNMENT_32 == 4
#pragma pack(4)
#endif

typedef struct statvfs64_32 {
	uint32_t	f_bsize;	/* preferred file system block size */
	uint32_t	f_frsize;	/* fundamental file system block size */
	fsblkcnt64_t	f_blocks;	/* total blocks of f_frsize */
	fsblkcnt64_t	f_bfree;	/* total free blocks of f_frsize */
	fsblkcnt64_t	f_bavail;	/* free blocks avail to non-superuser */
	fsfilcnt64_t	f_files;	/* total # of file nodes (inodes) */
	fsfilcnt64_t	f_ffree;	/* total # of free file nodes */
	fsfilcnt64_t	f_favail;	/* free nodes avail to non-superuser */
	uint32_t	f_fsid;		/* file system id (dev for now) */
	char		f_basetype[_FSTYPSZ];	/* target fs type name, */
						/* null-terminated */
	uint32_t	f_flag;		/* bit-mask of flags */
	uint32_t	f_namemax;	/* maximum file name length */
	char		f_fstr[32];	/* filesystem-specific string */
	uint32_t	f_filler[16];	/* reserved for future expansion */
} statvfs64_32_t;

#if _LONG_LONG_ALIGNMENT == 8 && _LONG_LONG_ALIGNMENT_32 == 4
#pragma	pack()
#endif

#endif	/* _SYSCALL32 */

/*
 * Flag definitions.
 */

#define	ST_RDONLY	0x01	/* read-only file system */
#define	ST_NOSUID	0x02	/* does not support setuid/setgid semantics */
#define	ST_NOTRUNC	0x04	/* does not truncate long file names */

#ifndef _KERNEL
int statvfs(const char *_RESTRICT_KYWD, struct statvfs *_RESTRICT_KYWD);
int fstatvfs(int, struct statvfs *);

/*
 * FIXME: source compat; statvfs64() is an alias of statvfs(), but the argument
 * types have different names (struct statvfs vs struct statvfs64) even if
 * identical, so we can't simply add the function decl here - use a #define
 * instead.
 */
#define statvfs64 statvfs
#define statvfs64_t statvfs_t
int fstatvfs64(int, struct statvfs *);
#endif

#ifdef	__cplusplus
}
#endif

#endif	/* _SYS_STATVFS_H */
