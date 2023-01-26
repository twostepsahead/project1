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
 * Copyright 2007 Sun Microsystems, Inc.  All rights reserved.
 * Use is subject to license terms.
 */

/*	Copyright (c) 1983, 1984, 1985, 1986, 1987, 1988, 1989 AT&T	*/
/*	  All Rights Reserved  	*/

/*
 * Portions of this source code were derived from Berkeley 4.3 BSD
 * under license from the Regents of the University of California.
 */

#pragma ident	"%Z%%M%	%I%	%E% SMI"

#include <sys/param.h>
#include <sys/isa_defs.h>
#include <sys/types.h>
#include <sys/sysmacros.h>
#include <sys/cred.h>
#include <sys/systm.h>
#include <sys/errno.h>
#include <sys/ttold.h>
#include <sys/vnode.h>
#include <sys/file.h>
#include <sys/mode.h>
#include <sys/proc.h>
#include <sys/uio.h>
#include <sys/kmem.h>
#include <sys/filio.h>
#include <sys/sunddi.h>
#include <sys/debug.h>
#include <sys/int_limits.h>
#include <sys/model.h>

/*
 * I/O control.
 */

int
ioctl(int fdes, int cmd, intptr_t arg)
{
	file_t *fp;
	int error = 0;
	vnode_t *vp;
	struct vattr vattr;
	int32_t flag;
	int rv = 0;

	if ((fp = getf(fdes)) == NULL)
		return (set_errno(EBADF));
	vp = fp->f_vnode;

	if (vp->v_type == VREG || vp->v_type == VDIR) {
		/*
		 * Handle these two ioctls for regular files and
		 * directories.  All others will usually be failed
		 * with ENOTTY by the VFS-dependent code.  System V
		 * always failed all ioctls on regular files, but SunOS
		 * supported these.
		 */
		switch (cmd) {
		case FIONREAD: {
			/*
			 * offset is int32_t because that is what FIONREAD
			 * is defined in terms of.  We cap at INT_MAX as in
			 * other cases for this ioctl.
			 */
			int32_t offset;

			vattr.va_mask = VATTR_SIZE;
			error = fop_getattr(vp, &vattr, 0, fp->f_cred, NULL);
			if (error) {
				releasef(fdes);
				return (set_errno(error));
			}
			offset = MIN(vattr.va_size - fp->f_offset, INT_MAX);
			if (copyout(&offset, (caddr_t)arg, sizeof (offset))) {
				releasef(fdes);
				return (set_errno(EFAULT));
			}
			releasef(fdes);
			return (0);
			}

		case FIONBIO:
			if (copyin((caddr_t)arg, &flag, sizeof (flag))) {
				releasef(fdes);
				return (set_errno(EFAULT));
			}
			mutex_enter(&fp->f_tlock);
			if (flag)
				fp->f_flag |= FNDELAY;
			else
				fp->f_flag &= ~FNDELAY;
			mutex_exit(&fp->f_tlock);
			releasef(fdes);
			return (0);

		default:
			break;
		}
	}

	/*
	 * ioctl() now passes in the model information in some high bits.
	 *
	 * FIXME: This is a terrible hack to preserve the original behavior
	 * before we combined f_flag and f_flag2 into a single field.  The
	 * proper fix would be to pass the file_t pointer to the fop_ioctl
	 * function instead of the vnode.  The data model could be passed in
	 * by itself.  (get_udatamodel uses curproc so we can't push it into
	 * the individual ioctl functions or we'd break kernel-issued
	 * ioctls.)
	 */
	flag = (fp->f_flag & 0xffff) | get_udatamodel();
	error = fop_ioctl(fp->f_vnode, cmd, arg, flag, CRED(), &rv, NULL);
	if (error != 0) {
		releasef(fdes);
		return (set_errno(error));
	}
	switch (cmd) {
	case FIONBIO:
		if (copyin((caddr_t)arg, &flag, sizeof (flag))) {
			releasef(fdes);
			return (set_errno(EFAULT));
		}
		mutex_enter(&fp->f_tlock);
		if (flag)
			fp->f_flag |= FNDELAY;
		else
			fp->f_flag &= ~FNDELAY;
		mutex_exit(&fp->f_tlock);
		break;

	default:
		break;
	}
	releasef(fdes);
	return (rv);
}
