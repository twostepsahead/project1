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
 * Copyright 2014 Garrett D'Amore <garrett@damore.org>
 *
 * Copyright 2004 Sun Microsystems, Inc.  All rights reserved.
 * Use is subject to license terms.
 */

#ifndef _SYS_INT_CONST_H
#define	_SYS_INT_CONST_H

/*
 * This file, <sys/int_const.h>, is part of the Sun Microsystems implementation
 * of <inttypes.h> as proposed in the ISO/JTC1/SC22/WG14 C committee's working
 * draft for the revision of the current ISO C standard, ISO/IEC 9899:1990
 * Programming language - C.
 *
 * Programs/Modules should not directly include this file.  Access to the
 * types defined in this file should be through the inclusion of one of the
 * following files:
 *
 *	<sys/inttypes.h>	Provides the Kernel and Driver appropriate
 *				components of <inttypes.h>.
 *
 *	<inttypes.h>		For use by applications.
 *
 * See these files for more details.
 *
 * Use at your own risk.  This file will track the evolution of the revision
 * of the current ISO C standard.  As of February 1996, the committee is
 * squarely behind the fixed sized types.
 */

/*
 * Constants
 *
 * The following macros create constants of the types defined in
 * <sys/int_types.h>. The intent is that:
 *	Constants defined using these macros have a specific size and
 *	signedness. The suffix used for int64_t and uint64_t (ll and ull)
 *	are for examples only. Implementations are permitted to use other
 *	suffixes.
 */
#define	__CONCAT__(A,B) A ## B

#define	INT8_C(c)	(c)
#define	INT16_C(c)	(c)
#define	INT32_C(c)	(c)
#define	INT64_C(c)	__CONCAT__(c,LL)

#define	UINT8_C(c)	(c)
#define	UINT16_C(c)	(c)
#define	UINT32_C(c)	__CONCAT__(c,U)
#define	UINT64_C(c)	__CONCAT__(c,ULL)

#define	INTMAX_C(c)	__CONCAT__(c,LL)
#define	UINTMAX_C(c)	__CONCAT__(c,ULL)

#endif /* _SYS_INT_CONST_H */
