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
# Copyright 2008 Sun Microsystems, Inc.  All rights reserved.
# Use is subject to license terms.
#

LIBRARY = libproject.a
VERS = .1

OBJECTS = \
	getprojent.o \
	project_walk.o \
	setproject.o

# include library definitsion
include ../../Makefile.lib

SRCDIR =	../common

LIBS =		$(DYNLIB)

CERRWARN +=	-Wno-parentheses
CERRWARN +=	-Wno-uninitialized
LDLIBS +=	-lc -lsecdb -lproc -lpool

.KEEP_STATE:

all: $(LIBS)

# include library targets
include ../../Makefile.targ

pics/%.o: ../common/%.c
	$(COMPILE.c) -o $@ $<
	$(POST_PROCESS_O)
