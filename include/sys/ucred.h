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
 * Copyright 2009 Sun Microsystems, Inc.  All rights reserved.
 * Use is subject to license terms.
 *
 * File with private definitions for the ucred structure for use by the
 * kernel and library routines.
 */

#ifndef	_SYS_UCRED_H
#define	_SYS_UCRED_H

#include <sys/types.h>
#include <sys/procfs.h>
#include <sys/cred.h>
#include <sys/priv.h>

#ifndef _KERNEL
#include <unistd.h>
#endif

#ifdef	__cplusplus
extern "C" {
#endif



#if defined(_KERNEL) || _STRUCTURED_PROC != 0
/*
 * bitness neutral struct
 *
 * Add new fixed fields at the end of the structure.
 */
struct ucred_s {
	uint32_t	uc_size;	/* Size of the full structure */
	uint32_t	uc_credoff;	/* Credential offset: 0 - no cred */
	uint32_t	uc_privoff;	/* Privilege offset: 0 - no privs */
	pid_t		uc_pid;		/* Process id */
	zoneid_t	uc_zoneid;	/* Zone id */
	projid_t	uc_projid;	/* Project id */
					/* The rest goes here */
};

/* Get the process credentials */
#define	UCCRED(uc)	(prcred_t *)(((uc)->uc_credoff == 0) ? NULL : \
				((char *)(uc)) + (uc)->uc_credoff)

/* Get the process privileges */
#define	UCPRIV(uc)	(prpriv_t *)(((uc)->uc_privoff == 0) ? NULL : \
				((char *)(uc)) + (uc)->uc_privoff)

#endif /* _KERNEL || _STRUCTURED_PROC != 0 */

/*
 * SYS_ucredsys subcodes.
 */
#define	UCREDSYS_UCREDGET	0
#define	UCREDSYS_GETPEERUCRED	1

#ifdef _KERNEL

extern uint32_t ucredminsize(const cred_t *);

#define	UCRED_PRIV_OFF	(sizeof (struct ucred_s))
/* The prcred_t has a variable size; it should be last. */
#define	UCRED_CRED_OFF	(UCRED_PRIV_OFF + priv_prgetprivsize(NULL))

#define	UCRED_SIZE	(UCRED_CRED_OFF + sizeof (prcred_t) + \
			    (ngroups_max - 1) * sizeof (gid_t))


struct proc;

extern struct ucred_s *pgetucred(struct proc *);
extern struct ucred_s *cred2ucred(const cred_t *, pid_t, void *,
    const cred_t *);

#else

/* Definition only valid for structured proc. */
#if _STRUCTURED_PROC != 0
#define	UCRED_SIZE(ip)	(sizeof (struct ucred_s) + sizeof (prcred_t) + \
			((int)sysconf(_SC_NGROUPS_MAX) - 1) * sizeof (gid_t) + \
			sizeof (prpriv_t) + \
			sizeof (priv_chunk_t) * \
			((ip)->priv_setsize * (ip)->priv_nsets - 1) + \
			(ip)->priv_infosize)
#endif

extern struct ucred_s *_ucred_alloc(void);

#endif

#ifdef	__cplusplus
}
#endif

#endif	/* _SYS_UCRED_H */
