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
# Copyright 2006 Sun Microsystems, Inc.  All rights reserved.
# Use is subject to license terms.
#
# Copyright (c) 2018, Joyent, Inc.

.KEEP_STATE:
.SUFFIXES:

SRCS += fmtopo.c
OBJS = $(SRCS:%.c=%.o)

PROG = fmtopo
ROOTLIBFM = $(ROOT)/usr/lib/fm
ROOTLIBFMD = $(ROOT)/usr/lib/fm/fmd
ROOTPROG = $(ROOTLIBFMD)/$(PROG)

$(NOT_RELEASE_BUILD)CPPFLAGS += -DDEBUG
CPPFLAGS += -I. -I../common
CFLAGS64 += $(CTF_FLAGS)
LDLIBS64 += -L$(ROOT)/usr/lib/fm -ltopo -lnvpair
LDFLAGS += -R/usr/lib/fm

# not linted
SMATCH=off

.NO_PARALLEL:
.PARALLEL: $(OBJS)

all: $(PROG)

$(PROG): $(OBJS)
	$(LINK.c) $(OBJS) -o $@ $(LDLIBS)
	$(CTFMERGE) -L VERSION -o $@ $(OBJS)
	$(POST_PROCESS)

%.o: ../common/%.c
	$(COMPILE.c) $<
	$(CTFCONVERT_O)

%.o: %.c
	$(COMPILE.c) $<
	$(CTFCONVERT_O)

clean:
	$(RM) $(OBJS)

clobber: clean
	$(RM) $(PROG)

$(ROOTLIBFMD)/%: %
	$(INS.file)

install_h:

install: all $(ROOTPROG)