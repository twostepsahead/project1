#
# Copyright 2007 Sun Microsystems, Inc.  All rights reserved.
#  Use is subject to license terms.
#
# Copyright (c) 2018, Joyent, Inc.

PROG=		chat
OBJS=		chat.o
SRCS=		$(OBJS:%.o=%.c)

include		../../../Makefile.cmd

CPPFLAGS +=	-DSOL2
LDFLAGS +=	$(MAPFILE.NGB:%=-Wl,-M%)
CERRWARN +=	-Wno-unused-value
CERRWARN +=	-Wno-trigraphs

# not linted
SMATCH=off

.KEEP_STATE:

all: $(PROG)

install: all $(ROOTPROG)

clean:
	$(RM) $(OBJS)


include		../../../Makefile.targ
