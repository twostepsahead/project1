#
# This file and its contents are supplied under the terms of the
# Common Development and Distribution License ("CDDL"), version 1.0.
# You may only use this file in accordance with the terms version
# 1.0 of the CDDL.
#
# A full copy of the text of the CDDL should have accompanied this
# source.  A copy is of the CDDL is also available via the Internet
# at http://www.illumos.org/license/CDDL.
#

#
# Copyright 2010 Chris Love.  All rights reserved.
# Copyright (c) 2018, Joyent, Inc.
#


PROG=		tail

OBJS=		forward.o misc.o read.o reverse.o tail.o
SRCS=	 	$(OBJS:%.o=%.c)

include ../Makefile.cmd

CLOBBERFILES=	$(PROG)

CSTD=	$(CSTD_GNU99)

SMOFF += leaks

# install rules
$(ROOTINC)/% : %
	$(INS.file)

.KEEP_STATE:

.PARALLEL: $(OBJS)

all:		$(PROG)

$(PROG):	$(OBJS)
	$(LINK.c) $(OBJS) -o $@ $(LDLIBS)
	$(POST_PROCESS)

install: all .WAIT $(ROOTPROG)


clean:
	$(RM) $(OBJS)

include ../Makefile.targ
