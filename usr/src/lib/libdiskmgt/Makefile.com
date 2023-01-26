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
# Copyright 2009 Sun Microsystems, Inc.  All rights reserved.
# Use is subject to license terms.
# Copyright 2016 Nexenta Systems, Inc.
# Copyright (c) 2018, Joyent, Inc.

LIBRARY =	libdiskmgt.a
VERS =		.1
OBJECTS =	alias.o \
		assoc_types.o \
		bus.o \
		cache.o \
		controller.o \
		drive.o \
		entry.o \
		events.o \
		findevs.o \
		inuse_dump.o \
		inuse_fs.o \
		inuse_lu.o \
		inuse_mnt.o \
		inuse_vxvm.o \
		inuse_zpool.o \
		media.o \
		partition.o \
		path.o \
		slice.o

include ../../Makefile.lib

LIBS =		$(DYNLIB)
i386_LDLIBS =   -lfdisk
sparc_LDLIBS =
LDLIBS +=       -ldevinfo -ladm -ldevid -lkstat -lsysevent \
		-lnvpair -lefi -lc $($(MACH)_LDLIBS)

SRCDIR =	../common

CERRWARN +=	-Wno-switch
CERRWARN +=	-Wno-parentheses
CERRWARN +=	-Wno-uninitialized
CPPFLAGS +=	-I$(SRC)/lib/libdiskmgt/common 

# not linted
SMATCH=off

.KEEP_STATE:

all: $(LIBS)

include ../../Makefile.targ
