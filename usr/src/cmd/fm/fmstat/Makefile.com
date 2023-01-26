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

.KEEP_STATE:
.SUFFIXES:

SRCS += fmstat.c

OBJS = $(SRCS:%.c=%.o)

PROG = fmstat
ROOTPROG = $(ROOTUSRSBIN)/$(PROG)

STATCOMMONDIR = $(SRC)/cmd/stat/common

STAT_COMMON_OBJS = timestamp.o

$(NOT_RELEASE_BUILD)CPPFLAGS += -DDEBUG
CPPFLAGS += -I. -I../common -I$(STATCOMMONDIR)
CFLAGS64 += $(CTF_FLAGS)
LDLIBS64 += -L$(ROOT)/usr/lib/fm -lfmd_adm
LDFLAGS += -R/usr/lib/fm

SMOFF += signed

.NO_PARALLEL:
.PARALLEL: $(OBJS)

all: $(PROG)

$(PROG): $(OBJS) $(STAT_COMMON_OBJS)
	$(LINK.c) $(OBJS)  $(STAT_COMMON_OBJS) -o $@ $(LDLIBS)
	$(CTFMERGE) -L VERSION -o $@ $(OBJS) $(STAT_COMMON_OBJS)
	$(POST_PROCESS)

%.o: ../common/%.c
	$(COMPILE.c) $<
	$(CTFCONVERT_O)

%.o: %.c
	$(COMPILE.c) $<
	$(CTFCONVERT_O)

%.o : $(STATCOMMONDIR)/%.c
	$(COMPILE.c) $<
	$(CTFCONVERT_O)

clean:
	$(RM) $(OBJS) $(STAT_COMMON_OBJS)

clobber: clean
	$(RM) $(PROG)

install_h:

install: all $(ROOTPROG)
