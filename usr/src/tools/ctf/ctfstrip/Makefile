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

.KEEP_STATE:

PROG = ctfstrip
SRCS = \
	ctfstrip.c

include $(SRC)/tools/Makefile.tools
CERRWARN += -Wno-unused-variable

OBJS = $(SRCS:%.c=%.o)


.NO_PARALLEL:
.PARALLEL: $(OBJS)

all: $(PROG)

$(PROG): $(OBJS)
	$(LINK.c) $(OBJS) -o $@ $(LDLIBS)
	$(POST_PROCESS)

$(ROOTONBLDMACHPROG): $(PROG)

install: $(ROOTONBLDMACHPROG)

clean:
	$(RM) $(OBJS)

include $(SRC)/tools/Makefile.targ
