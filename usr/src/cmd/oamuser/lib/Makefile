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
# Copyright (c) 2018, Joyent, Inc.

LIBRARY= 	lib.a

DATEFILE= 	ugdates
DATEFILESRC= 	ugdates.dat
TXT= 		$(DATEFILESRC)

OBJECTS= 	putgrent.o \
		errmsg.o \
		file.o \
		vgid.o \
		vgname.o \
		vgroup.o \
		vuid.o \
		vlogin.o \
		vproj.o \
		dates.o \
		vexpire.o \
		putprojent.o \
		vprojid.o \
		vprojname.o

# include library definitions
include ../../Makefile.cmd
include ../../../lib/Makefile.lib

SRCDIR =	.

GREP= 		grep
FILEMODE=	$(LIBFILEMODE)

PRODUCT=	$(LIBRARY) $(DATEFILE)

CLOBBERFILES=	$(DATEFILE)

GENERAL=	../inc
CPPFLAGS=	-I. -I$(GENERAL) $(CPPFLAGS.master)
CERRWARN +=	-Wno-parentheses
CERRWARN +=	-Wno-type-limits
CERRWARN +=	-Wno-unused-variable
# not linted
SMATCH=off

ARFLAGS=	cr
AROBJS=		`$(LORDER) $(OBJS) | $(TSORT)`

CLOBBERFILES += $(LIBRARY)

.KEEP_STATE:

all:		$(PRODUCT) $(TXT)

$(DATEFILE):	$(DATEFILESRC)
	$(GREP) -v "^#ident" $(DATEFILESRC) > $(DATEFILE)

install:	all $(DATEFILE)

include ../../../lib/Makefile.targ
