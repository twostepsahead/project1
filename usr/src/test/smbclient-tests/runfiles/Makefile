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
# Copyright (c) 2012 by Delphix. All rights reserved.
#

include $(SRC)/Makefile.master

SRCS =	default.run \
	just-nsmbrc.run \
	just-sharectl.run \
	just-smbfs.run \
	just-smbmount.run \
	just-smbutil.run \
	perf-regression.run

ROOTOPTPKG = $(ROOT)/opt/smbclient-tests
RUNFILES = $(ROOTOPTPKG)/runfiles

CMDS = $(SRCS:%=$(RUNFILES)/%)
$(CMDS) := FILEMODE = 0444

all: $(SRCS)

install: $(CMDS)

clean clobber:

$(CMDS): $(RUNFILES) $(SRCS)

$(RUNFILES):
	$(INS.dir)

$(RUNFILES)/%: %
	$(INS.file)
