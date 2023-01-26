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

LIBRARY=	libzonecfg.a
VERS=		.1
OBJECTS=	libzonecfg.o getzoneent.o scratchops.o

include ../../Makefile.lib

LIBS =		$(DYNLIB)
LDLIBS +=	-lc -luuid -lnvpair -lsysevent -lsec -lbrand \
		-lpool -lscf -lproc -luutil -lsecdb -lxml2

SRCDIR =	../common
CPPFLAGS +=	-I$(ADJUNCT_PROTO)/usr/include/libxml2 -I$(SRCDIR)
CERRWARN +=	-Wno-uninitialized
CERRWARN +=	-Wno-parentheses

.KEEP_STATE:

all:	$(LIBS)

include ../../Makefile.targ
