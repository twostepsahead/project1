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
# Copyright (c) 2018, Joyent, Inc.

PROG= init 
ROOTFS_PROG= $(PROG)

DEFAULTFILES= init.dfl

include ../Makefile.cmd

CSTD = $(CSTD_GNU99)
LDLIBS += -lpam -lcontract -lscf
CERRWARN += -Wno-parentheses
CERRWARN += -Wno-uninitialized
CLOBBERFILES= $(STATIC)

# needs work
SMATCH=off

.KEEP_STATE:

all: $(ROOTFS_PROG)

install: all $(ROOTETCDEFAULTFILES) $(ROOTSBINPROG)
	$(RM) $(ROOTUSRSBINPROG)
	$(SYMLINK) ../../sbin/$(PROG) $(ROOTUSRSBINPROG)
	$(RM) $(ROOTETC)/TIMEZONE
	cd $(ROOTETC); $(SYMLINK) default/init TIMEZONE

test:
	rtest $(PROG)

clean:

include ../Makefile.targ
