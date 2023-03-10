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

LIBRARY= libiscsit.a
VERS= .1

ISCSIT_OBJS_SHARED=	iscsit_common.o
ISCSI_OBJS_SHARED=	base64.o
OBJS_COMMON=		libiscsit.o
OBJECTS= $(OBJS_COMMON) $(ISCSIT_OBJS_SHARED) $(ISCSI_OBJS_SHARED)

include ../../Makefile.lib

LIBS=	$(DYNLIB)

SRCDIR =	../common

INCS += -I$(SRCDIR)
INCS += -I$(SRCTOP)/include

CSTD=	$(CSTD_GNU99)
LDLIBS +=	-lc -lnvpair -lstmf -luuid -lscf
CPPFLAGS +=	$(INCS)
CERRWARN +=	-Wno-parentheses

SRCS=	$(OBJS_COMMON:%.o=$(SRCDIR)/%.c)			\
	$(ISCSIT_OBJS_SHARED:%.o=$(SRC)/common/iscsit/%.c)	\
	$(ISCSI_OBJS_SHARED:%.o=$(SRC)/common/iscsi/%.c)

.KEEP_STATE:

all: $(LIBS)

pics/%.o: ../../../common/iscsit/%.c
	$(COMPILE.c) -o $@ $<
	$(POST_PROCESS_O)

pics/%.o: ../../../common/iscsi/%.c
	$(COMPILE.c) -o $@ $<
	$(POST_PROCESS_O)

include ../../Makefile.targ
