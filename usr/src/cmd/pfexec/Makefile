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
# Copyright 2014 Garrett D'Amore <garrett@damore.org>
# Copyright (c) 1999, 2010, Oracle and/or its affiliates. All rights reserved.
#

PROG= pfexec
SRCS= $(PROG:%=%.c)
PFEXEC= pfexec

BINSHELLS = bash ksh rksh sh zsh

include ../Makefile.cmd

FILEMODE =	0555

ROOTBINPFEXEC = $(PFEXEC:%=$(ROOTBIN)/%)

.KEEP_STATE:

all: $(PROG) 

install: all $(ROOTBINPFEXEC)
	for s in $(BINSHELLS); do \
		$(RM) $(ROOTBIN)/pf$$s; \
		$(LN) $(ROOTBIN)/pfexec $(ROOTBIN)/pf$$s; \
	done
	$(RM) $(ROOTSBIN)/pfsh
	$(SYMLINK) ../usr/bin/pfexec $(ROOTSBIN)/pfsh

clean:

include ../Makefile.targ
