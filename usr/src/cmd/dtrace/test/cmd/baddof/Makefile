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
# Copyright 2007 Sun Microsystems, Inc.  All rights reserved.
# Use is subject to license terms.
#
# Copyright (c) 2018, Joyent, Inc.

include $(SRC)/cmd/Makefile.cmd

.KEEP_STATE:

ROOTOPTPKG = $(ROOT)/opt/SUNWdtrt
ROOTBIN = $(ROOTOPTPKG)/bin

LDFLAGS += $(ZNODEFS) $(ZTEXTOFF)
LDLIBS = $(LDLIBS.cmd)
LDLIBS += -ldtrace

SRCS = baddof
CMDS = $(SRCS:%=$(ROOTBIN)/%)

CERRWARN += -Wno-implicit-function-declaration

# not linted
SMATCH=off

$(CMDS) := FILEMODE = 0555

all: $(SRCS)

install: $(CMDS)

clean:

clobber:
	$(RM) $(SRCS)

$(CMDS): $(ROOTBIN) $(SRCS)

$(ROOTBIN):
	$(INS.dir)

$(ROOTBIN)/%: %
	$(INS.file)
