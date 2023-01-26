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
#
# cmd/ls/Makefile.com
#

PROG=		ls
OBJS=           $(PROG).o
SRCS=           $(OBJS:%.o=../%.c)

include ../../Makefile.cmd

LDLIBS += -lsec -lnvpair -lcmdutils -lncurses

# Include all XPG4 changes in the XPG6 version
CFLAGS += -DXPG4 -DXPG6
CFLAGS64 += -DXPG4 -DXPG6


.KEEP_STATE:

all:	$(PROG)


clean:
	$(RM) $(CLEANFILES)

include ../../Makefile.targ

%: ../%.c
	$(LINK.c) -o $@ $< $(LDLIBS)
	$(POST_PROCESS)
