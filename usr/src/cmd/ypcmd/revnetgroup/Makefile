#
# CDDL HEADER START
#
# The contents of this file are subject to the terms of the
# Common Development and Distribution License, Version 1.0 only
# (the "License").  You may not use this file except in compliance
# with the License.
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
# Copyright 1996, 2002 Sun Microsystems, Inc.  All rights reserved.
# Use is subject to license terms.
#
# Copyright (c) 2018, Joyent, Inc.

PROG = revnetgroup

include ../../Makefile.cmd

OBJS = revnetgroup.o getgroup.o table.o util.o
SRCS = $(OBJS:.o=.c)
HDRS = getgroup.h table.h util.h

CERRWARN += -Wno-implicit-function-declaration
CERRWARN += -Wno-parentheses
CERRWARN += -Wno-uninitialized

# not linted
SMATCH=off

.KEEP_STATE:

all: $(PROG)

$(PROG): $(OBJS)
	$(LINK.c) $(OBJS) -o $@ $(LDLIBS)
	$(POST_PROCESS)

install: all $(ROOTUSRSBINPROG)

clean:
	$(RM) $(OBJS)

cstyle:
	${CSTYLE} ${SRCS}

clobber: clean
	$(RM) $(PROG)

FRC:
