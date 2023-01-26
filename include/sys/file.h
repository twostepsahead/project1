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
 * Copyright (c) 1989, 2010, Oracle and/or its affiliates. All rights reserved.
 */

/*	Copyright (c) 1984, 1986, 1987, 1988, 1989 AT&T	*/
/*	  All Rights Reserved	*/

/* Copyright (c) 2013, OmniTI Computer Consulting, Inc. All rights reserved. */
/* Copyright 2015 Joyent, Inc. */

#ifndef _SYS_FILE_H
#define	_SYS_FILE_H

#include <sys/t_lock.h>
#include <sys/fcntl.h>
#ifdef _KERNEL
#include <sys/model.h>
#include <sys/user.h>
#endif

#ifdef	__cplusplus
extern "C" {
#endif

/*
 * fio locking:
 *   f_rwlock	protects f_vnode and f_cred
 *   f_tlock	protects the rest
 *
 *   The purpose of locking in this layer is to keep the kernel
 *   from panicing if, for example, a thread calls close() while
 *   another thread is doing a read().  It is up to higher levels
 *   to make sure 2 threads doing I/O to the same file don't
 *   screw each other up.
 */
/*
 * One file structure is allocated for each open/creat/pipe call.
 * Main use is to hold the read/write pointer (and OFD locks) associated with
 * each open file.
 */
typedef struct file {
	kmutex_t	f_tlock;	/* short term lock */
	uint32_t	f_flag;
	struct vnode	*f_vnode;	/* pointer to vnode structure */
	offset_t	f_offset;	/* read/write character pointer */
	struct cred	*f_cred;	/* credentials of user who opened it */
	struct f_audit_data	*f_audit_data;	/* file audit data */
	int		f_count;	/* reference count */
	struct filock *f_filock;	/* ptr to single lock_descriptor_t */
} file_t;

/*
 * fpollinfo struct - used by poll caching to track who has polled the fd
 */
typedef struct fpollinfo {
	struct _kthread		*fp_thread;	/* thread caching poll info */
	struct fpollinfo	*fp_next;
} fpollinfo_t;


#ifdef _KERNEL

/*
 * Fake flags for driver ioctl calls to inform them of the originating
 * process' model.  See <sys/model.h>
 *
 * Part of the Solaris 2.6+ DDI/DKI
 */
#define	FMODELS	DATAMODEL_MASK	/* Note: 0x0ff00000 */
#define	FILP32	DATAMODEL_ILP32
#define	FLP64	DATAMODEL_LP64
#define	FNATIVE	DATAMODEL_NATIVE

#define	OFFSET_MAX(fd)	(MAXOFFSET_T)

/*
 * Fake flag => internal ioctl call for layered drivers.
 * Note that this flag deliberately *won't* fit into
 * the f_flag field of a file_t.
 *
 * Part of the Solaris 2.x DDI/DKI.
 */
#define	FKIOCTL		0x80000000	/* ioctl addresses are from kernel */

/*
 * Fake flag => this time to specify that the open(9E)
 * comes from another part of the kernel, not userland.
 *
 * Part of the Solaris 2.x DDI/DKI.
 */
#define	FKLYR		0x40000000	/* layered driver call */

#endif	/* _KERNEL */

/* miscellaneous defines */

#ifndef L_SET
#define	L_SET	0	/* for lseek */
#endif /* L_SET */

/*
 * For flock(3C).  These really don't belong here but for historical reasons
 * the interface defines them to be here.
 */
#define	LOCK_SH	1
#define	LOCK_EX	2
#define	LOCK_NB	4
#define	LOCK_UN	8

#if !defined(_STRICT_SYMBOLS)
extern int flock(int, int);
#endif

#if defined(_KERNEL)

/*
 * Routines dealing with user per-open file flags and
 * user open files.
 */
struct proc;	/* forward reference for function prototype */
struct vnodeops;
struct vattr;

extern file_t *getf(int);
extern void releasef(int);
extern void areleasef(int, uf_info_t *);
#ifndef	_BOOT
extern void closeall(uf_info_t *);
#endif
extern void flist_fork(uf_info_t *, uf_info_t *);
extern int closef(file_t *);
extern int closeandsetf(int, file_t *);
extern int ufalloc_file(int, file_t *);
extern int ufalloc(int);
extern int ufcanalloc(struct proc *, uint_t);
extern int falloc(struct vnode *, int, file_t **, int *);
extern void finit(void);
extern void unfalloc(file_t *);
extern void setf(int, file_t *);
extern int f_getfd_error(int, int *);
extern char f_getfd(int);
extern int f_setfd_error(int, int);
extern void f_setfd(int, char);
extern int f_getfl(int, int *);
extern int f_badfd(int, int *, int);
extern int fassign(struct vnode **, int, int *);
extern void fcnt_add(uf_info_t *, int);
extern void close_exec(uf_info_t *);
extern void clear_stale_fd(void);
extern void clear_active_fd(int);
extern void free_afd(afd_t *afd);
extern int fgetstartvp(int, char *, struct vnode **);
extern int fsetattrat(int, char *, int, struct vattr *);
extern int fisopen(struct vnode *);
extern void delfpollinfo(int);
extern void addfpollinfo(int);
extern int sock_getfasync(struct vnode *);
extern int files_can_change_zones(void);
#ifdef DEBUG
/* The following functions are only used in ASSERT()s */
extern void checkwfdlist(struct vnode *, fpollinfo_t *);
extern void checkfpollinfo(void);
extern int infpollinfo(int);
#endif	/* DEBUG */

#endif	/* defined(_KERNEL) */

#ifdef	__cplusplus
}
#endif

#endif	/* _SYS_FILE_H */
