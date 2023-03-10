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
 * Copyright 2005 Sun Microsystems, Inc.  All rights reserved.
 * Use is subject to license terms.
 */

/*	Copyright (c) 1986 AT&T	*/
/*	  All Rights Reserved  	*/

/*
 * Copyright 2010 Nexenta Systems, Inc.  All rights reserved.
 */

/*	This module is created for NLS on Sep.03.86		*/

/*
 * Ungetwc saves the process code c into the one character buffer
 * associated with an input stream "iop". That character, c,
 * will be returned by the next getwc call on that stream.
 */

#include "lint.h"
#include "file64.h"
#include <stdio.h>
#include <stdlib.h>
#include <widec.h>
#include <limits.h>
#include <errno.h>
#include "libc.h"
#include "stdiom.h"
#include "mse.h"

static wint_t
__ungetwc_impl(wint_t wc, FILE *iop)
{
	char		mbs[MB_LEN_MAX];
	unsigned char	*p;
	int		n;
	rmutex_t	*lk;

	FLOCKFILE(lk, iop);

	if (GET_NO_MODE(iop)) {
		_setorientation(iop, _WC_MODE);
	}
	if ((wc == WEOF) || ((iop->_flag & _IOREAD) == 0)) {
		FUNLOCKFILE(lk);
		return (WEOF);
	}

	n = wctomb(mbs, (wchar_t)wc);
	if (n <= 0) {
		FUNLOCKFILE(lk);
		return (WEOF);
	}

	if (iop->_ptr <= iop->_base) {
		if (iop->_base == NULL) {
			FUNLOCKFILE(lk);
			return (WEOF);
		}
		if (iop->_ptr == iop->_base && iop->_cnt == 0) {
			++iop->_ptr;
		} else if ((iop->_ptr - n) < (iop->_base - PUSHBACK)) {
			FUNLOCKFILE(lk);
			return (WEOF);
		}
	}

	p = (unsigned char *)(mbs + n - 1);
	while (n--) {
		*--(iop)->_ptr = (*p--);
		++(iop)->_cnt;
	}
	iop->_flag &= ~_IOEOF;
	FUNLOCKFILE(lk);
	return (wc);
}

wint_t
ungetwc(wint_t wc, FILE *iop)
{
	return (__ungetwc_impl(wc, iop));
}
