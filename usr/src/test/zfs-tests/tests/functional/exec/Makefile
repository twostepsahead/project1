#
# This file and its contents are supplied under the terms of the
# Common Development and Distribution License ("CDDL"), version 1.0.
# You may only use this file in accordance with the terms of version
# 1.0 of the CDDL.
#
# A full copy of the text of the CDDL should have accompanied this
# source.  A copy of the CDDL is also available via the Internet at
# http://www.illumos.org/license/CDDL.
#

#
# Copyright (c) 2013 by Delphix. All rights reserved.
#

include $(SRC)/Makefile.master

ROOTOPTPKG = $(ROOT)/opt/zfs-tests
TESTDIR = $(ROOTOPTPKG)/tests/functional/exec

PROG = mmap_exec

SCRIPTS = cleanup \
	exec_001_pos \
	exec_002_neg \
	setup

include $(SRC)/cmd/Makefile.cmd
include $(SRC)/test/Makefile.com

ROOTOPTPKG = $(ROOT)/opt/zfs-tests

OBJS = $(PROG:%=%.o)
SRCS = $(OBJS:%.o=%.c)

CMDS = $(PROG:%=$(TESTDIR)/%) $(SCRIPTS:%=$(TESTDIR)/%)
$(CMDS) := FILEMODE = 0555


all: $(PROG)

$(PROG): $(OBJS)
	$(LINK.c) $(OBJS) -o $@ $(LDLIBS)
	$(POST_PROCESS)

%.o: ../%.c
	$(COMPILE.c) $<

install: all $(CMDS)

clobber: clean
	-$(RM) $(PROG)

clean:
	-$(RM) $(OBJS)

$(CMDS): $(TESTDIR) $(PROG)

$(TESTDIR):
	$(INS.dir)

$(TESTDIR)/%: %
	$(INS.file)

$(TESTDIR)/%: %.ksh
	$(INS.rename)
