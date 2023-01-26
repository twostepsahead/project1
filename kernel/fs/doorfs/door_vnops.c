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
/*
 * Copyright (c) 2017 by Delphix. All rights reserved.
 */

#include <sys/types.h>
#include <sys/vnode.h>
#include <sys/vfs.h>
#include <sys/door.h>
#include <sys/proc.h>
#include <sys/kmem.h>
#include <sys/debug.h>
#include <sys/cmn_err.h>
#include <sys/fs_subr.h>
#include <sys/zone.h>

kmutex_t	door_knob;
static int	door_open(struct vnode **vpp, int flag, struct cred *cr,
			caller_context_t *ct);
static int	door_close(struct vnode *vp, int flag, int count,
			offset_t offset, struct cred *cr, caller_context_t *ct);
static int	door_getattr(struct vnode *vp, struct vattr *vap,
			int flags, struct cred *cr, caller_context_t *ct);
static void	door_inactive(struct vnode *vp, struct cred *cr,
			caller_context_t *ct);
static int	door_access(struct vnode *vp, int mode, int flags,
			struct cred *cr, caller_context_t *ct);
static int	door_realvp(vnode_t *vp, vnode_t **vpp, caller_context_t *ct);

struct vfs door_vfs;

const struct vnodeops door_vnodeops = {
	.vnop_name = "doorfs",
	.vop_open = door_open,
	.vop_close = door_close,
	.vop_getattr = door_getattr,
	.vop_access = door_access,
	.vop_inactive = door_inactive,
	.vop_frlock = fs_nosys,
	.vop_realvp = door_realvp,
	.vop_poll = fs_nosys_poll,
	.vop_pathconf = fs_nosys,
	.vop_dispose = fs_nodispose,
	.vop_getsecattr = fs_nosys,
	.vop_shrlock = fs_nosys,
};

/* ARGSUSED */
static int
door_open(struct vnode **vpp, int flag, struct cred *cr, caller_context_t *ct)
{
	return (0);
}

/* ARGSUSED */
static int
door_close(struct vnode *vp, int flag, int count, offset_t offset,
    struct cred *cr, caller_context_t *ct)
{
	door_node_t	*dp = VTOD(vp);

	/*
	 * If this is being called from closeall on exit, any doors created
	 * by this process should have been revoked already in door_exit.
	 */
	ASSERT(dp->door_target != curproc ||
	    ((curthread->t_proc_flag & TP_LWPEXIT) == 0));

	/*
	 * Deliver an unref if needed.
	 *
	 * If the count is equal to 2, it means that I'm doing a fop_close
	 * on the next to last reference for *this* file struct. There may
	 * be multiple files pointing to this vnode in which case the v_count
	 * will be > 1.
	 *
	 * The door_active count is bumped during each invocation.
	 */
	if (count == 2 && vp->v_count == 1 &&
	    (dp->door_flags & (DOOR_UNREF | DOOR_UNREF_MULTI))) {
		mutex_enter(&door_knob);
		if (dp->door_active == 0) {
			/* o.k. to deliver unref now */
			door_deliver_unref(dp);
		} else {
			/* do the unref later */
			dp->door_flags |= DOOR_DELAY;
		}
		mutex_exit(&door_knob);
	}
	return (0);
}

/* ARGSUSED */
static int
door_getattr(struct vnode *vp, struct vattr *vap, int flags, struct cred *cr,
    caller_context_t *ct)
{
	static timestruc_t tzero = {0, 0};
	extern dev_t doordev;

	vap->va_mask = 0;		/* bit-mask of attributes */
	vap->va_type = vp->v_type;	/* vnode type (for create) */
	vap->va_mode = 0777;		/* file access mode */
	vap->va_uid = 0;		/* owner user id */
	vap->va_gid = 0;		/* owner group id */
	vap->va_fsid = doordev;		/* file system id (dev for now) */
	vap->va_nodeid = (ino64_t)0;		/* node id */
	vap->va_nlink = vp->v_count;	/* number of references to file */
	vap->va_size = 0;		/* file size in bytes */
	vap->va_atime = tzero;		/* time of last access */
	vap->va_mtime = tzero;		/* time of last modification */
	vap->va_ctime = tzero;		/* time file ``created'' */
	vap->va_rdev = doordev;		/* device the file represents */
	vap->va_blksize = 0;		/* fundamental block size */
	vap->va_nblocks = (fsblkcnt64_t)0;	/* # of blocks allocated */
	vap->va_seq = 0;		/* sequence number */

	return (0);
}

/* ARGSUSED */
static void
door_inactive(struct vnode *vp, struct cred *cr, caller_context_t *ct)
{
	door_node_t *dp = VTOD(vp);

	mutex_enter(&vp->v_lock);
	/*
	 * Once the door_node is unreferenced, it stays unreferenced,
	 * so we can simply return if there are active thread bindings;
	 * the final door_unbind_thread() will re-invoke us.
	 */
	ASSERT(vp->v_count == 1);
	if (dp->door_bound_threads > 0) {
		VN_RELE_LOCKED(vp);
		mutex_exit(&vp->v_lock);
		return;
	}
	mutex_exit(&vp->v_lock);

	/* if not revoked, remove door from per-process list */
	if (dp->door_target) {
		mutex_enter(&door_knob);
		if (dp->door_target)	/* recheck door_target under lock */
			door_list_delete(dp);
		mutex_exit(&door_knob);
	}
	vn_invalid(vp);
	vn_free(vp);
	kmem_free(dp, sizeof (door_node_t));
}

/*
 * To avoid having bound threads interfere with unref processing, we
 * don't use VN_HOLD/VN_RELE to track threads bound to our private
 * pool.  Instead, we keep a separate counter, also under v_lock.
 */
void
door_bind_thread(door_node_t *dp)
{
	vnode_t *vp = DTOV(dp);

	mutex_enter(&vp->v_lock);
	dp->door_bound_threads++;
	ASSERT(dp->door_bound_threads > 0 && vp->v_count > 0);
	mutex_exit(&vp->v_lock);
}

void
door_unbind_thread(door_node_t *dp)
{
	vnode_t *vp = DTOV(dp);
	int do_inactive = 0;

	mutex_enter(&vp->v_lock);
	ASSERT(dp->door_bound_threads > 0);
	if (--dp->door_bound_threads == 0 && vp->v_count == 0) {
		/* set up for inactive handling */
		VN_HOLD_LOCKED(vp);
		do_inactive = 1;
	}
	mutex_exit(&vp->v_lock);

	if (do_inactive)
		door_inactive(vp, NULL, NULL);
}

/* ARGSUSED */
static int
door_access(struct vnode *vp, int mode, int flags, struct cred *cr,
    caller_context_t *ct)
{
	return (0);
}

/* ARGSUSED */
static int
door_realvp(vnode_t *vp, vnode_t **vpp, caller_context_t *ct)
{
	*vpp = vp;
	return (0);
}
