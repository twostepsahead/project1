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

PROG=		rm
RMOBJ1=		rm.o
RMOBJ=		$(RMOBJ1) getresponse.o
SRCS= 		$(RMOBJ1:%.o=%.c) $(SRC)/common/util/getresponse.c

include ../Makefile.cmd

CERRWARN += -Wno-parentheses
CFLAGS += -DXPG4
CPPFLAGS +=  -I $(SRC)/common/util
.KEEP_STATE:

all: $(PROG)

install: all $(ROOTPROG)

$(PROG): $(RMOBJ) 
	$(LINK.c) $(RMOBJ) -o $@ $(LDLIBS)
	$(POST_PROCESS)

%.o:    $(SRC)/common/util/%.c
	$(COMPILE.c) $(OUTPUT_OPTION) $<
	$(POST_PROCESS_O)

clean:
	$(RM) $(RMOBJ)


include ../Makefile.targ
