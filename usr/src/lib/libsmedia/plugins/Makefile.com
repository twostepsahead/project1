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
# Copyright (c) 2018, Joyent, Inc.

LIBS =		$(DYNLIB)

CPPFLAGS +=	-I../inc -I$(SRC)/cmd/smserverd/
LDLIBS +=	-lc $(PLUGIN_SPECIFIC_LIB)

PLUGINDIR = $(ROOTLIBDIR64)/smedia/$(MACH32)
PLUGINDIR64 = $(ROOTLIBDIR64)/smedia
FILEMODE = 555

SOFILES	= $(LIBRARY:%.a=%.so)
PLUGINS  = $(LIBS:%=$(PLUGINDIR)/%)
PLUGINS64  = $(LIBS:%=$(PLUGINDIR64)/%)

SRCDIR =	../common

# not linted
SMATCH=off

.KEEP_STATE:

objs/%.o pics/%.o: ../common/%.c
	$(COMPILE.c) -o $@ $<
	$(POST_PROCESS_O)

$(PLUGINDIR) :
	${INS.dir}

$(PLUGINDIR64) :
	${INS.dir}

$(PLUGINDIR)/% : %
	${INS.file}

$(PLUGINDIR64)/% : %
	${INS.file}

