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
# Copyright (c) 2018, Joyent, Inc.

MANIFEST =	zonestat.xml
SVCMETHOD =	svc-zonestat
PROG =		zonestatd

include ../../Makefile.cmd

ROOTCMDDIR =		$(ROOTLIB64)/zones
ROOTMANIFESTDIR =	$(ROOTSVCSYSTEM)

SRCS =	zonestatd.c

CPPFLAGS	+= -I$(ADJUNCT_PROTO)/usr/include/libxml2
LDLIBS 		+= -lkstat -lpool -lexacct -lscf \
		    -lcontract -lcmdutils -lumem

CERRWARN 	+= -Wno-parentheses
CERRWARN 	+= -Wno-unused-label
CERRWARN 	+= -Wno-uninitialized

# not linted
SMATCH=off

OBJS =		$(SRCS:%.c=%.o)

.KEEP_STATE:

.PARALLEL:

all: $(PROG)

install: all $(ROOTCMD) $(ROOTMANIFEST) $(ROOTSVCMETHOD)

check: $(CHKMANIFEST)

clean:
	$(RM) $(OBJS)


include ../../Makefile.targ
