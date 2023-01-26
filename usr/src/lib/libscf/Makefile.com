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
# Copyright (c) 2004, 2010, Oracle and/or its affiliates. All rights reserved.
#
# Copyright (c) 2018, Joyent, Inc.

LIBRARY =	libscf.a
VERS =		.1

OBJECTS = \
	error.o			\
	lowlevel.o		\
	midlevel.o		\
	notify_params.o		\
	highlevel.o		\
	scf_tmpl.o		\
	scf_type.o

include ../../Makefile.lib
include ../../Makefile.rootfs

LIBS =		$(DYNLIB)

$(NOT_NATIVE)NATIVE_BUILD = $(POUND_SIGN)
$(NATIVE_BUILD)VERS =
$(NATIVE_BUILD)LIBS = $(DYNLIB)

LDLIBS_i386 += -lsmbios
LDLIBS +=	-luutil -lc -lgen -lnvpair
LDLIBS +=	$(LDLIBS_$(MACH))

SRCDIR =	../common

COMDIR =	../../../common/svc

CFLAGS +=	$(CSTD_GNU99)
CPPFLAGS +=	-I../inc -I../../common/inc -I$(COMDIR) -I$(ROOTHDRDIR)
$(NOT_RELEASE_BUILD) CPPFLAGS += -DFASTREBOOT_DEBUG

CERRWARN +=	-Wno-switch
CERRWARN +=	-Wno-char-subscripts
CERRWARN +=	-Wno-unused-label
CERRWARN +=	-Wno-parentheses
CERRWARN +=	-Wno-uninitialized

# not linted
SMATCH=off

MY_NATIVE_CPPFLAGS =\
		-DNATIVE_BUILD $(DTEXTDOM) \
		-I../inc -I$(COMDIR)
MY_NATIVE_LDLIBS = -luutil -lc -lgen -lnvpair
MY_NATIVE_LDLIBS_i386 = -lsmbios
MY_NATIVE_LDLIBS += $(MY_NATIVE_LDLIBS_$(MACH))

.KEEP_STATE:

all:

include ../../Makefile.targ
