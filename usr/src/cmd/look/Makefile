#
# CDDL HEADER START
#
# The contents of this file are subject to the terms of the
# Common Development and Distribution License, Version 1.0 only
# (the "License").  You may not use this file except in compliance
# with the License.
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
# Copyright (c) 1989 by Sun Microsystems, Inc.
#
# Copyright (c) 2018, Joyent, Inc.

PROG= look
DATADIR= dict
DATA= words

include ../Makefile.cmd

ROOTDATADIR=	$(ROOTSHLIB)/$(DATADIR)
ROOTDATA=	$(DATA:%=$(ROOTDATADIR)/%)
# Targetdirs installs /usr/lib.  Don't re-install it
#DIRS=		$(ROOTLIB) $(ROOTDATADIR)
DIRS=		$(ROOTDATADIR)

CERRWARN +=	-Wno-implicit-function-declaration

# not linted
SMATCH=off

FILEMODE=	755

$(ROOTDATADIR)/$(DATA):= FILEMODE=444

.KEEP_STATE:

$(ROOTDATADIR)/% : % $(ROOTDATADIR)
	$(INS.file)

all: $(PROG) $(DATA)

install: all $(ROOTPROG) $(DIRS) $(ROOTDATA)

$(DIRS):
	$(INS.dir)

clean:


include ../Makefile.targ

