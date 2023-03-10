#
# This file and its contents are supplied under the terms of the
# Common Development and Distribution License ("CDDL"), version 1.0.
# You may only use this file in accordance with the terms of version
# 1.0 of the CDDL.
#
# A full copy of the text of the CDDL should have accompanied this
# source.  A copy of the CDDL is also available via the Internet at
# http://www.illumos.org/license/CDDL.
#

#
# Copyright (c) 2012, Igor Kozhukhov <ikozhukhov@gmail.com>.  All rights reserved.
#

<include system-library-math.man3m.inc>
<include system-library-math.man3mvec.inc>
dir path=lib
dir path=lib/$(ARCH32)
dir path=lib/$(ARCH32)/libmvec
dir path=usr group=sys
dir path=usr/include
dir path=usr/include/iso
dir path=usr/include/sys
dir path=usr/lib
dir path=usr/lib/$(ARCH32)
dir path=usr/share/man/man3m
dir path=usr/share/man/man3mvec
file path=lib/$(ARCH32)/libm.so.2
file path=lib/$(ARCH32)/libmvec.so.1
file path=lib/$(ARCH32)/libmvec/libmvec_hwcap1.so.1
file path=lib/libm.so.2
file path=lib/libmvec.so.1
file path=usr/include/complex.h
file path=usr/include/fenv.h
file path=usr/include/floatingpoint.h
file path=usr/include/iso/math_c99.h
file path=usr/include/iso/math_iso.h
file path=usr/include/math.h
file path=usr/include/sys/ieeefp.h
file path=usr/include/tgmath.h
link path=lib/$(ARCH32)/libm.so target=libm.so.2
link path=lib/$(ARCH32)/libmvec.so target=libmvec.so.1
link path=lib/libm.so target=libm.so.2
link path=lib/libmvec.so target=libmvec.so.1
link path=usr/lib/$(ARCH32)/libm.so target=../../../lib/$(ARCH32)/libm.so.2
link path=usr/lib/$(ARCH32)/libm.so.2 target=../../../lib/$(ARCH32)/libm.so.2
link path=usr/lib/$(ARCH32)/libmvec.so \
    target=../../../lib/$(ARCH32)/libmvec.so.1
link path=usr/lib/$(ARCH32)/libmvec.so.1 \
    target=../../../lib/$(ARCH32)/libmvec.so.1
link path=usr/lib/libm.so target=../../lib/libm.so.2
link path=usr/lib/libm.so.2 target=../../lib/libm.so.2
link path=usr/lib/libmvec.so target=../../lib/libmvec.so.1
link path=usr/lib/libmvec.so.1 target=../../lib/libmvec.so.1
