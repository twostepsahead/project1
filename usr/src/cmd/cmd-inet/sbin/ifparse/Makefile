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
# Makefile for "ifparse" 
#
# Copyright 2014 Garrett D'Amore <garrett@damore.org>
#
# Copyright 2000-2003 Sun Microsystems, Inc.  All rights reserved.
# Use is subject to license terms.
#

PROG =		ifparse
ROOTFS_PROG =	$(PROG)
OBJS=		ifparse.o
SRCS=		$(OBJS:%.o=%.c)

include ../../../Makefile.cmd
include ../../Makefile.cmd-inet

CPPFLAGS += -I$(CMDINETCOMMONDIR)

# these #defines are required to use UNIX 98 interfaces
$(OBJS)		:= CPPFLAGS +=-D_POSIX_C_SOURCE=199506L

.KEEP_STATE:

all:	$(ROOTFS_PROG)

$(PROG):	$(OBJS)
	$(LINK.c) $(OBJS) -o $@ $(LDLIBS)
	$(POST_PROCESS)

install:	all $(ROOTSBINPROG)

clean:
	$(RM) $(OBJS)


include ../../../Makefile.targ
