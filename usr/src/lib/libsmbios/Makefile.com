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
# Copyright 2006 Sun Microsystems, Inc.  All rights reserved.
# Use is subject to license terms.
#

LIBRARY = libsmbios.a
VERS = .1

COMMON_OBJS = \
	smbios_error.o \
	smbios_info.o \
	smbios_open.o

LIB_OBJS = \
	smbios_lib.o \
	smbios_subr.o \
	smbios_tables.o

OBJECTS = $(COMMON_OBJS) $(LIB_OBJS)

include ../../Makefile.lib

COMMON_SRCDIR = ../../../common/smbios
COMMON_HDR = $(SRCTOP)/include/sys/smbios.h

SRCS = $(COMMON_OBJS:%.o=$(COMMON_SRCDIR)/%.c) $(LIB_OBJS:%.o=../common/%.c)
LIBS = $(DYNLIB)

SRCDIR = ../common

CLEANFILES += ../common/smbios_tables.c

CPPFLAGS += -I../common -I$(COMMON_SRCDIR)
LDLIBS += -ldevinfo -lc

.KEEP_STATE:

all: $(LIBS)

include ../../Makefile.targ

objs/%.o pics/%.o: ../../../common/smbios/%.c
	$(COMPILE.c) -o $@ $<
	$(POST_PROCESS_O)

../common/smbios_tables.c: $(COMMON_SRCDIR)/mktables.sh $(COMMON_HDR)
	sh $(COMMON_SRCDIR)/mktables.sh $(COMMON_HDR) > $@
