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
# Copyright (c) 2013 by Delphix. All rights reserved.
#

MODULE = genunix.so
MDBTGT = kvm

include ../../../common/modules/genunix/Makefile.files

COMMONSRCS = \
	$(GENUNIX_SRCS)

KMODSRCS = \
	$(COMMONSRCS)

MODSRCS = \
	$(COMMONSRCS) \
	typegraph.c

#
# This signals that $(KMODSRCS) != $(MODSRCS).  Typegraph is not usable under
# kmdb.  As such, we don't bother compiling it.
KMOD_SOURCES_DIFFERENT=$(POUND_SIGN)

include ../../../../Makefile.cmd
include ../../../../Makefile.cmd.64
include ../../Makefile.amd64
include ../../../Makefile.module

dmod/$(MODULE) := LDLIBS += -lm -lproc

#
# We are not actually hardwiring some dependency on x86, we just need to
# include sys/param.h with _MACHDEP defined, and this forces the inclusion of
# machparam.h, even though we don't use anything there.  This is a temporary
# kludge until we invent -DDONTINCLUDEMACHPARAM or something.
#
CPPFLAGS += -I$(SRCTOP)/arch/x86/kernel/include

CPPFLAGS += -I$(SRC)/uts/common
CPPFLAGS += -I$(SRCTOP)/include

# Needed to find include file mutex_impl.h
CPPFLAGS += -I$(SRC)/uts/intel

CERRWARN += -Wno-char-subscripts
CERRWARN += -Wno-unused-label
CERRWARN += -Wno-uninitialized
CERRWARN += -Wno-parentheses
CERRWARN += -Wno-type-limits


MODSRCS_DIR = ../../../intel/modules/genunix
