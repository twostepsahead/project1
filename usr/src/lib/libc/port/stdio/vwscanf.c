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
 * Copyright 2008 Sun Microsystems, Inc.  All rights reserved.
 * Use is subject to license terms.
 */

#pragma ident	"%Z%%M%	%I%	%E% SMI"

#include "lint.h"
#include "file64.h"
#include <mtlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <thread.h>
#include <synch.h>
#include <wchar.h>
#include <errno.h>
#include <stdlib.h>
#include <alloca.h>
#include "mse.h"
#include "stdiom.h"
#include "libc.h"

int
vwscanf(const wchar_t *fmt, va_list ap)
{
	rmutex_t	*lk;
	int	ret;

	FLOCKFILE(lk, stdin);

	if (GET_NO_MODE(stdin))
		_setorientation(stdin, _WC_MODE);

	ret = __wdoscan_u(stdin, fmt, ap, 0);
	FUNLOCKFILE(lk);
	return (ret);
}

int
vfwscanf(FILE *iop, const wchar_t *fmt, va_list ap)
{
	rmutex_t	*lk;
	int	ret;

	FLOCKFILE(lk, iop);

	if (GET_NO_MODE(iop))
		_setorientation(iop, _WC_MODE);

	ret = __wdoscan_u(iop, fmt, ap, 0);
	FUNLOCKFILE(lk);
	return (ret);
}

int
vswscanf(const wchar_t *wstr, const wchar_t *fmt, va_list ap)
{
	FILE	strbuf;
	size_t	wlen, clen;
	char	*tmp_buf;
	int	ret;

	/*
	 * The dummy FILE * created for swscanf has the _IOWRT
	 * flag set to distinguish it from wscanf and fwscanf
	 * invocations.
	 */

	clen = wcstombs(NULL, wstr, 0);
	if (clen == (size_t)-1) {
		errno = EILSEQ;
		return (EOF);
	}
	tmp_buf = alloca(sizeof (char) * (clen + 1));
	if (tmp_buf == NULL)
		return (EOF);
	wlen = wcstombs(tmp_buf, wstr, clen + 1);
	if (wlen == (size_t)-1) {
		errno = EILSEQ;
		return (EOF);
	}

	strbuf._flag = _IOREAD | _IOWRT;
	strbuf._ptr = strbuf._base = (unsigned char *)tmp_buf;
	strbuf._cnt = strlen(tmp_buf);
	SET_FILE(&strbuf, _NFILE);

	/* Probably the following is not required. */
	/* _setorientation(&strbuf, _WC_MODE); */

	ret = __wdoscan_u(&strbuf, fmt, ap, 0);
	return (ret);
}
