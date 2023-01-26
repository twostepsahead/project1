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
/*
 * Copyright 2003 Sun Microsystems, Inc.  All rights reserved.
 * Use is subject to license terms.
 */

#ifndef _SYS_IB_MGT_IBMF_IBMF_TRACE_H
#define	_SYS_IB_MGT_IBMF_IBMF_TRACE_H

#pragma ident	"%Z%%M%	%I%	%E% SMI"

/*
 * This file contains the IBMF trace/debug macros.
 */

#ifdef __cplusplus
extern "C" {
#endif

#include <sys/tnf_probe.h>

#define	IBMF_TNF_ERROR		"ibmf ibmf_error"
#define	IBMF_TNF_TRACE		"ibmf ibmf_trace"

/*
 * Trace levels for printing
 */
#define	DPRINT_L0	0	/* no messages */
#define	DPRINT_L1	1	/* major errors */
#define	DPRINT_L2	2	/* minor errors */
#define	DPRINT_L3	3	/* general debug */
#define	DPRINT_L4	4	/* general trace */

/*
 * Trace probe macros
 */
#define	IBMF_TNF_NODEBUG	0
#define	IBMF_TNF_DEBUG		1

#define	IBMF_TRACE_0(trlevel, arg04)	\
	if (ibmf_trace_level > 0) {				\
		ibmf_dprintf(trlevel, arg04);			\
	}

/* CSTYLED */
#define	IBMF_TRACE_1(trlevel, arg04, arg13)		\
	if (ibmf_trace_level > 0) {				\
		ibmf_dprintf(trlevel, arg04, arg13);		\
	}

/* CSTYLED */
#define	IBMF_TRACE_2(trlevel, arg04, arg13, arg23)	\
	if (ibmf_trace_level > 0) {				\
		ibmf_dprintf(trlevel, arg04, arg13, arg23);	\
	}

/* CSTYLED */
#define	IBMF_TRACE_3(trlevel, arg04, arg13,arg23, arg33)		\
	if (ibmf_trace_level > 0) {				\
		ibmf_dprintf(trlevel, arg04, arg13, arg23, arg33);	\
	}

/* CSTYLED */
#define	IBMF_TRACE_4(trlevel, arg04, arg13, arg23, arg33, arg43)	\
	if (ibmf_trace_level > 0) {				\
		ibmf_dprintf(trlevel, arg04, arg13, arg23, arg33, arg43);\
	}

/* CSTYLED */
#define	IBMF_TRACE_5(trlevel, arg04, arg13, arg23, arg33, arg43, arg53)	\
	if (ibmf_trace_level > 0) {				\
		ibmf_dprintf(trlevel, arg04, arg13, arg23, arg33, \
		    arg43, arg53);				\
	}

void
ibmf_dprintf(int l, const char *fmt, ...);

#ifdef __cplusplus
}
#endif

#endif /* _SYS_IB_MGT_IBMF_IBMF_TRACE_H */
