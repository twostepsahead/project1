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
# Copyright 2008 Sun Microsystems, Inc.  All rights reserved.
# Use is subject to license terms.
#
# Copyright (c) 2012 by Delphix. All rights reserved.
# Copyright (c) 2014, Joyent, Inc.
#

LIBRARY=	libnvpair.a
VERS=		.1

OBJECTS=	libnvpair.o \
		nvpair_alloc_system.o \
		nvpair_alloc_fixed.o \
		nvpair.o \
		fnvpair.o \
		nvpair_json.o

include ../../Makefile.lib
include ../../Makefile.rootfs

SRCS=		../libnvpair.c \
		../nvpair_alloc_system.c \
		../nvpair_json.c \
		$(SRC)/common/nvpair/nvpair_alloc_fixed.c \
		$(SRC)/common/nvpair/nvpair.c \
		$(SRC)/common/nvpair/fnvpair.c

#
# Libraries added to the next line must be present in miniroot
#
LDLIBS +=	-lc
LIBS =		$(DYNLIB)

CERRWARN +=	-Wno-type-limits
CERRWARN +=	-Wno-parentheses
CERRWARN +=	-Wno-uninitialized


CSTD=	$(CSTD_GNU99)

.KEEP_STATE:

all: $(LIBS)

include ../../Makefile.targ

pics/%.o: $(SRC)/common/nvpair/%.c
	$(COMPILE.c) -o $@ $<
	$(POST_PROCESS_O)
