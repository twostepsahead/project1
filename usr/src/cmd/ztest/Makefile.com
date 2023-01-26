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
# Copyright (c) 2005, 2010, Oracle and/or its affiliates. All rights reserved.
# Copyright (c) 2012, 2016 by Delphix. All rights reserved.
# Copyright 2017 RackTop Systems.
# Copyright (c) 2018, Joyent, Inc.

PROG= ztest
OBJS= $(PROG).o
SRCS= $(OBJS:%.o=../%.c)

include ../../Makefile.cmd
include ../../Makefile.ctf

INCS += -I../../../lib/libzpool/common
INCS += -I$(SRCTOP)/kernel/fs/zfs
INCS += -I$(SRCTOP)/kernel/fs/zfs/lua
INCS += -I$(SRCTOP)/kernel/fs/zfs/common

LDLIBS += -lumem -lzpool -lcmdutils -lm -lnvpair

CSTD= $(CSTD_GNU99)
CFLAGS += -g
CFLAGS64 += -g
CPPFLAGS += $(INCS) -DDEBUG

CERRWARN += -Wno-switch

# false positive
SMOFF += signed

.KEEP_STATE:

all: $(PROG)

$(PROG): $(OBJS)
	$(LINK.c) -o $(PROG) $(OBJS) $(LDLIBS)
	$(POST_PROCESS)

clean:
	$(RM) $(OBJS)


include ../../Makefile.targ

%.o: ../%.c
	$(COMPILE.c) $<
	$(POST_PROCESS_O)
