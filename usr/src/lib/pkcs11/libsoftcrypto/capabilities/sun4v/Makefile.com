#
# CDDL HEADER START
#
# The contents of this file are subject to the terms of the
# Common Development and Distribution License (the "License").
# You may not use this file except in compliance with the License.
#
# You can obtain a copy of the license at usr/src/OPENSOLARIS.LICENSE
# or http://www.opensolaris.org/os/licensing.
# See the License for the specific language governing permissions
# and limitations under the License.
#
# When distributing Covered Code, include this CDDL HEADER in each
# file and include the License file at usr/src/OPENSOLARIS.LICENSE.
# If applicable, add the following below this CDDL HEADER, with the
# fields enclosed by brackets "[]" replaced with your own identifying
# information: Portions Copyright [yyyy] [name of copyright owner]
#
# CDDL HEADER END
#

#
# Copyright (c) 2010, Oracle and/or its affiliates. All rights reserved.
#

PLATFORM =	sun4v

include		../../Makefile.com

# Redefine the objects required for this capabilities group.
OBJECTS =	$(ARCFOUR_OBJS) $(BIGNUM_OBJS)

include		$(SRC)/lib/Makefile.lib

AS_CPPFLAGS +=	-D_ASM -DPIC -D$(MACH)
ASFLAGS +=	$(AS_PICFLAGS)
CFLAGS +=	-fno-strict-aliasing
CFLAGS +=	-fno-unit-at-a-time
CFLAGS +=	-fno-optimize-sibling-calls
CFLAGS +=	-O2
CFLAGS +=	-fbuiltin
CERRWARN +=	-Wno-parentheses
CERRWARN +=	-Wno-uninitialized
CERRWARN +=	-Wno-unused-variable
CERRWARN +=	-Wno-unused-function
CPPFLAGS +=	-D$(PLATFORM) -I$(CRYPTODIR) -I$(UTSDIR) \
		-D_POSIX_PTHREAD_SEMANTICS
BIGNUM_FLAGS +=	-DUMUL64 -DNO_BIG_ONE -DNO_BIG_TWO
