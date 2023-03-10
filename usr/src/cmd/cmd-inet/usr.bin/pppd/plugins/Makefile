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
# Copyright 2011 Nexenta Systems, Inc. All rights reserved.
#

MINCONN =	minconn.so
PASSPROMPT = 	passprompt.so
PPPOE =		pppoe.so

LIBRARIES =	minconn.so passprompt.so pppoe.so
OBJECTS =	minconn.o passprompt.o pppoe.o

include		$(SRC)/lib/Makefile.lib
include		../Makefile.def

# Express objects in terms of "pics" thus triggering the appropriate CPPFLAGS,
# CFLAGS and DYNFLAGS settings from lib/Makefile.lib to build shared objects.
$(MINCONN):=	PICS = pics/minconn.o
$(PASSPROMPT):=	PICS = pics/passprompt.o
$(PPPOE):=	PICS = pics/pppoe.o

# Suppress -h setting from DYNFLAGS as these libraries aren't linked against.
# Establish external references through mapfiles and dependencies to allow
# use of -zdefs.
HSONAME=
$(MINCONN):=	MAPFILES = mapfile-minconn
$(PASSPROMPT):=	MAPFILES = mapfile-passprompt
$(PPPOE):=	MAPFILES = mapfile-pppoe

$(PASSPROMPT) $(PPPOE):= LDLIBS += -lc

CPPFLAGS +=	-I.. -I$(SRC)/uts/common -I$(SRCTOP)/include
# XX64 -- this should not be needed -- fix me
DYNFLAGS +=	-nostdlib

CLOBBERFILES +=	$(LIBRARIES)

LIBPPPPLUGIN=	$(LIBRARIES:%=$(LIBPPPPLUGINDIR)/%)

$(LIBPPPPLUGIN):=	FILEMODE = 0544

$(LIBPPPPLUGINDIR):=	FILEMODE = 0755

.KEEP_STATE:

all:	$(LIBRARIES)

$(MINCONN): pics .WAIT $$(PICS)
	$(BUILD.SO)
	$(POST_PROCESS_SO)

$(PASSPROMPT): pics .WAIT $$(PICS)
	$(BUILD.SO)
	$(POST_PROCESS_SO)

$(PPPOE): pics .WAIT $$(PICS)
	$(BUILD.SO)
	$(POST_PROCESS_SO)

install: all $(LIBPPPPLUGINDIR) $(LIBPPPPLUGIN)

$(LIBPPPPLUGINDIR):
	$(INS.dir)

$(LIBPPPPLUGINDIR)/%:	%
	$(INS.file)

include $(SRC)/lib/Makefile.targ
