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
 * Copyright 2016 Joyent, Inc.
 */

/*
 * lx_sendfile() and lx_sendfile64() are primarily branded versions of the
 * library calls available in the Solaris libsendfile (see sendfile(3EXT)).
 */

#include <sys/types.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <sys/sendfile.h>
#include <string.h>
#include <errno.h>
#include <sys/lx_misc.h>
#include <sys/lx_syscall.h>
#include <sys/stat.h>

#if defined(_ILP32)
long
lx_sendfile(uintptr_t p1, uintptr_t p2, uintptr_t p3, uintptr_t p4)
{
	sysret_t rval;
	off_t off = 0;
	off_t *offp = (off_t *)p3;
	int error;
	struct sendfilevec sfv;
	size_t xferred = 0;
	size_t sz = (size_t)p4;

	if (offp == NULL) {
		/* If no offp, we must use the current offset */
		if ((off = lseek((int)p2, 0, SEEK_CUR)) == -1)
			return (-errno);
	} else {
		if (sz > 0 && uucopy(offp, &off, sizeof (off)) != 0)
			return (-errno);
		if (off < 0)
			return (-EINVAL);
	}

	sfv.sfv_fd = p2;
	sfv.sfv_flag = 0;
	sfv.sfv_off = off;
	sfv.sfv_len = sz;
	error = __systemcall(&rval, SYS_sendfilev, SENDFILEV, p1, &sfv,
	    1, &xferred);

	/* Suppress errors if we were able to write any data at all. */
	if (xferred > 0) {
		error = 0;
	}
	/*
	 * If we got EINVAL due to our offset being past EOF, also suppress
	 * errors (Linux just returns 0 here).
	 */
	if (error == EINVAL) {
		struct stat stat;
		if (fstat((int)p2, &stat) == 0) {
			if (off >= stat.st_size) {
				error = 0;
			}
		}
	}

	if (error == 0) {
		off += xferred;
		if (offp == NULL) {
			/* If no offp, we must adjust current offset */
			if (lseek((int)p2, off, SEEK_SET) == -1)
				return (-errno);
		} else {
			if (uucopy(&off, offp, sizeof (off)) != 0) {
				return (-EFAULT);
			}
		}
	}

	return (error ? -error : xferred);
}
#endif

long
lx_sendfile64(uintptr_t p1, uintptr_t p2, uintptr_t p3, uintptr_t p4)
{
	sysret_t rval;
	off64_t off = 0;
	off64_t *offp = (off64_t *)p3;
	size_t sz = (size_t)p4;
	int error;
	struct sendfilevec64 sfv;
	size_t xferred;

	if (offp == NULL) {
		/* If no offp, we must use the current offset */
		if ((off = lseek((int)p2, 0, SEEK_CUR)) == -1)
			return (-errno);
	} else {
		if (sz > 0 && uucopy(offp, &off, sizeof (off)) != 0)
			return (-errno);
		if (off < 0)
			return (-EINVAL);
	}

	sfv.sfv_fd = p2;
	sfv.sfv_flag = 0;
	sfv.sfv_off = off;
	sfv.sfv_len = sz;
	xferred = 0;
	error = __systemcall(&rval, SYS_sendfilev, SENDFILEV64, p1, &sfv,
	    1, &xferred);

	/* Suppress errors if we were able to write any data at all. */
	if (xferred > 0) {
		error = 0;
	}
	/*
	 * If we got EINVAL due to our offset being past EOF, also suppress
	 * errors (Linux just returns 0 here).
	 */
	if (error == EINVAL) {
		struct stat stat;
		if (fstat((int)p2, &stat) == 0) {
			if (off >= stat.st_size) {
				error = 0;
			}
		}
	}

	if (error == 0) {
		off += xferred;
		if (offp == NULL) {
			/* If no offp, we must adjust current offset */
			if (lseek((int)p2, off, SEEK_SET) == -1)
				return (-errno);
		} else {
			if (uucopy(&off, offp, sizeof (off)) != 0) {
				return (-EFAULT);
			}
		}
	}

	return (error ? -error : xferred);
}
