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
 * Copyright 2006 Sun Microsystems, Inc.  All rights reserved.
 * Use is subject to license terms.
 * Copyright 2017 Joyent, Inc.  All rights reserved.
 */

#ifndef	_LX_STATFS_H
#define	_LX_STATFS_H

#ifdef	__cplusplus
extern "C" {
#endif

extern int lx_statfs_init(void);

struct lx_statfs {
	size_t		f_type;
	size_t		f_bsize;
	ulong_t		f_blocks;
	ulong_t		f_bfree;
	ulong_t		f_bavail;
	ulong_t		f_files;
	ulong_t		f_ffree;
	u_longlong_t	f_fsid;
	size_t		f_namelen;
	size_t		f_frsize;
	size_t		f_spare[5];
};

struct lx_statfs64 {
	int		f_type;
	int		f_bsize;
	u_longlong_t	f_blocks;
	u_longlong_t	f_bfree;
	u_longlong_t	f_bavail;
	u_longlong_t	f_files;
	u_longlong_t	f_ffree;
	u_longlong_t	f_fsid;
	int		f_namelen;
	int		f_frsize;
	int		f_spare[5];
};

/*
 * These magic values are taken mostly from statfs(2) or magic.h
 */
#define	LX_AUTOFS_SUPER_MAGIC		0x0187
#define	LX_CGROUP_SUPER_MAGIC		0x27e0eb
#define	LX_DEVFS_SUPER_MAGIC		0x1373
#define	LX_DEVPTS_SUPER_MAGIC		0x1cd1
#define	LX_EXT2_SUPER_MAGIC		0xEF53
#define	LX_ISOFS_SUPER_MAGIC		0x9660
#define	LX_MSDOS_SUPER_MAGIC		0x4d44
#define	LX_NFS_SUPER_MAGIC		0x6969
#define	LX_PROC_SUPER_MAGIC		0x9fa0
#define	LX_SYSFS_SUPER_MAGIC		0x62656572
#define	LX_TMPFS_SUPER_MAGIC		0x01021994
#define	LX_UFS_MAGIC			0x00011954
#define	LX_PIPEFS_MAGIC			0x50495045

#ifdef	__cplusplus
}
#endif

#endif	/* _LX_STATFS_H */
