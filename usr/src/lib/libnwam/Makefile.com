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
# Copyright 2010 Sun Microsystems, Inc.  All rights reserved.
# Use is subject to license terms.
# Copyright (c) 2016, Chris Fraire <cfraire@me.com>.
#
# Copyright (c) 2018, Joyent, Inc.

LIBRARY=	libnwam.a
VERS=		.1
OBJECTS=	libnwam_backend.o \
		libnwam_enm.o \
		libnwam_events.o \
		libnwam_error.o \
		libnwam_files.o \
		libnwam_known_wlan.o \
		libnwam_loc.o \
		libnwam_ncp.o \
		libnwam_object.o \
		libnwam_util.o \
		libnwam_values.o \
		libnwam_wlan.o

include ../../Makefile.lib
include ../../Makefile.rootfs

LIBS =		$(DYNLIB)
LDLIBS +=	-lc -ldladm -lnvpair -lscf -lsecdb -lipadm

SRCDIR =	../common

CPPFLAGS +=	-I$(SRCDIR)

CERRWARN +=	-Wno-switch
CERRWARN +=	-Wno-uninitialized

# not linted
SMATCH=off

.KEEP_STATE:

all:	$(LIBS)

include  $(SRC)/lib/Makefile.targ
