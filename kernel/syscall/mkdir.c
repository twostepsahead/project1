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
 * Copyright (c) 1994, 2010, Oracle and/or its affiliates. All rights reserved.
 */

/*	Copyright (c) 1983, 1984, 1985, 1986, 1987, 1988, 1989 AT&T	*/
/*	  All Rights Reserved  	*/

/*
 * Portions of this source code were derived from Berkeley 4.3 BSD
 * under license from the Regents of the University of California.
 */

#include <sys/param.h>
#include <sys/isa_defs.h>
#include <sys/types.h>
#include <sys/sysmacros.h>
#include <sys/systm.h>
#include <sys/user.h>
#include <sys/errno.h>
#include <sys/vnode.h>
#include <sys/file.h>
#include <sys/fcntl.h>
#include <sys/uio.h>
#include <sys/debug.h>

/*
 * Make a directory.
 */
int
mkdirat(int fd, char *dname, int dmode)
{
	vnode_t *startvp;
	vnode_t *vp;
	struct vattr vattr;
	int error;

	vattr.va_type = VDIR;
	vattr.va_mode = dmode & PERMMASK;
	vattr.va_mask = VATTR_TYPE|VATTR_MODE;

	if (dname == NULL)
		return (set_errno(EFAULT));
	if ((error = fgetstartvp(fd, dname, &startvp)) != 0)
		return (set_errno(error));

	error = vn_createat(dname, UIO_USERSPACE, &vattr, EXCL, 0, &vp,
	    CRMKDIR, 0, PTOU(curproc)->u_cmask, startvp);
	if (startvp != NULL)
		VN_RELE(startvp);
	if (error)
		return (set_errno(error));
	VN_RELE(vp);
	return (0);
}

int
mkdir(char *dname, int dmode)
{
	return (mkdirat(AT_FDCWD, dname, dmode));
}
