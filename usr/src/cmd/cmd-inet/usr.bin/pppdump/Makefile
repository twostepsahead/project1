#
# Copyright (c) 2000, 2010, Oracle and/or its affiliates. All rights reserved.
# Copyright 2011 Nexenta Systems, Inc. All rights reserved.
# Copyright (c) 2018, Joyent, Inc.
#

PROG=		pppdump
OBJS=		bsd-comp.o deflate.o pppdump.o zlib.o
SRCS=		$(OBJS:%.o=%.c)

include		../../../Makefile.cmd

CPPFLAGS +=	-DPPP_DEFS_IN_NET

CERRWARN +=	-Wno-implicit-function-declaration
CERRWARN +=	-Wno-parentheses
CERRWARN +=	-Wno-unused-variable
CERRWARN +=	-Wno-uninitialized

# not linted
SMATCH=off

.KEEP_STATE:

all:		$(PROG)

$(PROG):	$(OBJS)
		$(LINK.c) $(OBJS) -o $@ $(LDLIBS)
		$(POST_PROCESS)

install:	 all $(ROOTPROG)

clean:
		$(RM) $(OBJS)


include		../../../Makefile.targ
