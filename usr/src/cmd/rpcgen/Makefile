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
# cmd/rpcgen/Makefile

PROG= rpcgen 


OBJS= rpc_clntout.o rpc_cout.o rpc_hout.o rpc_main.o rpc_parse.o \
	  rpc_scan.o rpc_svcout.o rpc_tblout.o rpc_util.o rpc_sample.o
SRCS= $(OBJS:%.o=%.c)

include ../Makefile.cmd

LDLIBS += 
CERRWARN +=	-Wno-switch
CERRWARN +=	-Wno-parentheses
CERRWARN +=	-Wno-char-subscripts

.KEEP_STATE:

all: $(PROG) 

$(PROG): $(OBJS)
	$(LINK.c) $(OBJS) -o $@ $(LDLIBS)
	$(POST_PROCESS)

install: all $(ROOTPROG)

clean:
	$(RM) $(OBJS)


include ../Makefile.targ
