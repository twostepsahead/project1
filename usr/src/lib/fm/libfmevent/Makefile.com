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
# Copyright (c) 2009, 2010, Oracle and/or its affiliates. All rights reserved.
#

LIBRARY = libfmevent.a
VERS = .1

LIBSRCS = fmev_subscribe.c \
	fmev_evaccess.c \
	fmev_errstring.c \
	fmev_util.c \
	fmev_publish.c

OBJECTS = $(LIBSRCS:%.c=%.o)

include ../../../Makefile.lib

# This library must install in /lib/fm since it is a dependency of
# svc.startd and may be required in early boot.  Thus we cannot
# include ../Makefile.lib - instead we set ROOTFMHDRDIR and
# ROOTFMHDRS and redefine ROOTLIBDIR and ROOTLIBDIR64 accordingly

ROOTFMHDRDIR = $(ROOTHDRDIR)/fm
ROOTFMHDRS   = $(FMHDRS:%=$(ROOTFMHDRDIR)/%)

ROOTLIBDIR=     $(ROOT)/lib/fm/$(MACH32)
ROOTLIBDIR64=   $(ROOT)/lib/fm

SRCS = $(LIBSRCS:%.c=../common/%.c)
LIBS = $(DYNLIB)

SRCDIR =	../common

CSTD = $(CSTD_GNU99)

CPPFLAGS += -I../common -I.
$(NOT_RELEASE_BUILD)CPPFLAGS += -DDEBUG

CFLAGS += $(C_BIGPICFLAGS)
CFLAGS64 += $(C_BIGPICFLAGS)

CERRWARN += -Wno-parentheses
CERRWARN += -Wno-unused-function
CERRWARN += -Wno-uninitialized

FMLIBDIR=usr/lib/fm/$(MACH32)
FMLIBDIR64=usr/lib/fm

$(DYNLIB) := LDLIBS += -lumem -lnvpair -luutil -lsysevent \
	-L$(ROOT)/$(FMLIBDIR) -ltopo -lc

$(BUILD64)$(DYNLIB) := LDLIBS64 += -lumem -lnvpair -luutil -lsysevent \
	-L$(ROOT)/$(FMLIBDIR64) -ltopo -lc



CLEANFILES += ../common/fmev_errstring.c

.KEEP_STATE:

all: $(LIBS)


pics/%.o: ../$(MACH)/%.c
	$(COMPILE.c) -o $@ $<
	$(POST_PROCESS_O)

../common/fmev_errstring.c: ../common/mkerror.sh ../common/libfmevent.h
	sh ../common/mkerror.sh ../common/libfmevent.h > $@

%.o: ../common/%.c
	$(COMPILE.c) -o $@ $<
	$(POST_PROCESS_O)

include ../../../Makefile.targ
include ../../Makefile.targ
