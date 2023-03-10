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
 * Copyright 2011 Nexenta Systems, Inc.  All rights reserved.
 */
/*
 * Copyright 2006 Sun Microsystems, Inc.  All rights reserved.
 * Use is subject to license terms.
 */

#pragma weak __remquol = remquol

#include "libm.h"
/* INDENT OFF */
static const int
	is = -0x7fffffff - 1,
	im = 0x0000ffff,
	iu = 0x00010000;

static const long double zero = 0.0L, one = 1.0L;
/* INDENT ON */

#error Unsupported architecture

/*
 * On entrance: *quo is initialized to 0, x finite and y non-zero & ordered
 */
static long double
fmodquol(long double x, long double y, int *quo) {
	long double a, b;
	int n, ix, iy, k, sx, sq, m;
	int hx;
	int x0, y0, z0, carry;
	unsigned x1, x2, x3, y1, y2, y3, z1, z2, z3;

	hx = __H0(x);
	x1 = __H1(x);
	x2 = __H2(x);
	x3 = __H3(x);
	y0 = __H0(y);
	y1 = __H1(y);
	y2 = __H2(y);
	y3 = __H3(y);

	sx = hx & is;
	sq = (hx ^ y0) & is;
	x0 = hx ^ sx;
	y0 &= ~0x80000000;

	a = fabsl(x);
	b = fabsl(y);
	if (a <= b) {
		if (a < b)
			return (x);
		else {
			*quo = 1 + (sq >> 30);
			return (zero * x);
		}
	}
	/* determine ix = ilogbl(x) */
	if (x0 < iu) {		/* subnormal x */
		ix = 0;
		ix = -16382;
		while (x0 == 0) {
			ix -= 16;
			x0 = x1 >> 16;
			x1 = (x1 << 16) | (x2 >> 16);
			x2 = (x2 << 16) | (x3 >> 16);
			x3 = (x3 << 16);
		}
		while (x0 < iu) {
			ix -= 1;
			x0 = (x0 << 1) | (x1 >> 31);
			x1 = (x1 << 1) | (x2 >> 31);
			x2 = (x2 << 1) | (x3 >> 31);
			x3 <<= 1;
		}
	} else {
		ix = (x0 >> 16) - 16383;
		x0 = iu | (x0 & im);
	}

	/* determine iy = ilogbl(y) */
	if (y0 < iu) {		/* subnormal y */
		iy = -16382;
		while (y0 == 0) {
			iy -= 16;
			y0 = y1 >> 16;
			y1 = (y1 << 16) | (y2 >> 16);
			y2 = (y2 << 16) | (y3 >> 16);
			y3 = (y3 << 16);
		}
		while (y0 < iu) {
			iy -= 1;
			y0 = (y0 << 1) | (y1 >> 31);
			y1 = (y1 << 1) | (y2 >> 31);
			y2 = (y2 << 1) | (y3 >> 31);
			y3 <<= 1;
		}
	} else {
		iy = (y0 >> 16) - 16383;
		y0 = iu | (y0 & im);
	}


	/* fix point fmod */
	n = ix - iy;
	m = 0;
	while (n--) {
		while (x0 == 0 && n >= 16) {
			m <<= 16;
			n -= 16;
			x0 = x1 >> 16;
			x1 = (x1 << 16) | (x2 >> 16);
			x2 = (x2 << 16) | (x3 >> 16);
			x3 = (x3 << 16);
		}
		while (x0 < iu && n >= 1) {
			m += m;
			n -= 1;
			x0 = (x0 << 1) | (x1 >> 31);
			x1 = (x1 << 1) | (x2 >> 31);
			x2 = (x2 << 1) | (x3 >> 31);
			x3 = (x3 << 1);
		}
		carry = 0;
		z3 = x3 - y3;
		carry = z3 > x3;
		if (carry == 0) {
			z2 = x2 - y2;
			carry = z2 > x2;
		} else {
			z2 = x2 - y2 - 1;
			carry = z2 >= x2;
		}
		if (carry == 0) {
			z1 = x1 - y1;
			carry = z1 > x1;
		} else {
			z1 = x1 - y1 - 1;
			carry = z1 >= x1;
		}
		z0 = x0 - y0 - carry;
		if (z0 < 0) {	/* double x */
			x0 = x0 + x0 + ((x1 & is) != 0);
			x1 = x1 + x1 + ((x2 & is) != 0);
			x2 = x2 + x2 + ((x3 & is) != 0);
			x3 = x3 + x3;
			m += m;
		} else {
			m += 1;
			if (z0 == 0) {
				if ((z1 | z2 | z3) == 0) {
					/* 0: we are done */
					if (n < 31)
						m <<= (1 + n);
					else
						m = 0;
					m &= ~0x80000000;
					*quo = sq >= 0 ? m : -m;
					__H0(a) = hx & is;
					__H1(a) = __H2(a) = __H3(a) = 0;
					return (a);
				}
			}
			/* x = z << 1 */
			z0 = z0 + z0 + ((z1 & is) != 0);
			z1 = z1 + z1 + ((z2 & is) != 0);
			z2 = z2 + z2 + ((z3 & is) != 0);
			z3 = z3 + z3;
			x0 = z0;
			x1 = z1;
			x2 = z2;
			x3 = z3;
			m += m;
		}
	}
	carry = 0;
	z3 = x3 - y3;
	carry = z3 > x3;
	if (carry == 0) {
		z2 = x2 - y2;
		carry = z2 > x2;
	} else {
		z2 = x2 - y2 - 1;
		carry = z2 >= x2;
	}
	if (carry == 0) {
		z1 = x1 - y1;
		carry = z1 > x1;
	} else {
		z1 = x1 - y1 - 1;
		carry = z1 >= x1;
	}
	z0 = x0 - y0 - carry;
	if (z0 >= 0) {
		x0 = z0;
		x1 = z1;
		x2 = z2;
		x3 = z3;
		m += 1;
	}
	m &= ~0x80000000;
	*quo = sq >= 0 ? m : -m;

	/* convert back to floating value and restore the sign */
	if ((x0 | x1 | x2 | x3) == 0) {
		__H0(a) = hx & is;
		__H1(a) = __H2(a) = __H3(a) = 0;
		return (a);
	}
	while (x0 < iu) {
		if (x0 == 0) {
			iy -= 16;
			x0 = x1 >> 16;
			x1 = (x1 << 16) | (x2 >> 16);
			x2 = (x2 << 16) | (x3 >> 16);
			x3 = (x3 << 16);
		} else {
			x0 = x0 + x0 + ((x1 & is) != 0);
			x1 = x1 + x1 + ((x2 & is) != 0);
			x2 = x2 + x2 + ((x3 & is) != 0);
			x3 = x3 + x3;
			iy -= 1;
		}
	}

	/* normalize output */
	if (iy >= -16382) {
		__H0(a) = sx | (x0 - iu) | ((iy + 16383) << 16);
		__H1(a) = x1;
		__H2(a) = x2;
		__H3(a) = x3;
	} else {		/* subnormal output */
		n = -16382 - iy;
		k = n & 31;
		if (k <= 16) {
			x3 = (x2 << (32 - k)) | (x3 >> k);
			x2 = (x1 << (32 - k)) | (x2 >> k);
			x1 = (x0 << (32 - k)) | (x1 >> k);
			x0 >>= k;
		} else {
			x3 = (x2 << (32 - k)) | (x3 >> k);
			x2 = (x1 << (32 - k)) | (x2 >> k);
			x1 = (x0 << (32 - k)) | (x1 >> k);
			x0 = 0;
		}
		while (n >= 32) {
			n -= 32;
			x3 = x2;
			x2 = x1;
			x1 = x0;
			x0 = 0;
		}
		__H0(a) = x0 | sx;
		__H1(a) = x1;
		__H2(a) = x2;
		__H3(a) = x3;
		a *= one;
	}
	return (a);
}

long double
remquol(long double x, long double y, int *quo) {
	int hx, hy, sx, sq;
	long double v;

	hx = __H0(x);		/* high word of x */
	hy = __H0(y);		/* high word of y */
	sx = hx & is;		/* sign of x */
	sq = (hx ^ hy) & is;	/* sign of x/y */
	hx ^= sx;		/* |x| */
	hy &= ~0x80000000;

	/* purge off exception values */
	*quo = 0;
	/* y=0, y is NaN, x is NaN or inf */
	if (y == 0.0L || y != y || hx >= 0x7fff0000)
		return ((x * y) / (x * y));

	y = fabsl(y);
	x = fabsl(x);
	if (hy <= 0x7ffdffff) {
		x = fmodquol(x, y + y, quo);
		*quo = ((*quo) & 0x3fffffff) << 1;
	}
	if (hy < 0x00020000) {
		if (x + x > y) {
			*quo += 1;
			if (x == y)
				x = zero;
			else
				x -= y;
			if (x + x >= y) {
				x -= y;
				*quo += 1;
			}
		}
	} else {
		v = 0.5L * y;
		if (x > v) {
			*quo += 1;
			if (x == y)
				x = zero;
			else
				x -= y;
			if (x >= v) {
				x -= y;
				*quo += 1;
			}
		}
	}
	if (sq != 0)
		*quo = -(*quo);
	return (sx == 0 ? x : -x);
}
