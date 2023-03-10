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

PROG =	rpc.smserverd
OBJS =	smediad.o smediad_err.o smed_svc.o smed_xdr.o
SRCS =	$(OBJS:%.o=%.c)

MANIFEST=	smserver.xml

DERIVED_FILES = smed.h smed_xdr.c smed_svc.c

# install specifics

ROOTLIBSMSERVERDIR= $(ROOTLIB64)/smedia
ROOTLIBSMSERVERD= $(PROG:%=$(ROOTLIBSMSERVERDIR)/%)

include ../Makefile.cmd

ROOTMANIFESTDIR=	$(ROOTSVCNETWORKRPC)

CERRWARN += -Wno-uninitialized
CERRWARN += -Wno-unused-variable

# not linted
SMATCH=off

.KEEP_STATE:

all: $(PROG)

install: all $(ROOTLIBSMSERVERDIR) $(ROOTLIBSMSERVERD) $(ROOTMANIFEST)

$(PROG):	$(DERIVED_FILES) $(OBJS)
	$(LINK.c) -o $(PROG) $(OBJS) $(LDLIBS)
	$(POST_PROCESS)

check:	$(CHKMANIFEST)

clean:
	$(RM) $(OBJS) $(DERIVED_FILES)


include ../Makefile.targ

$(ROOTLIBSMSERVERDIR):
	$(INS.dir)

$(ROOTLIBSMSERVERDIR)/%: %
	$(INS.file)

#
# Derived files
#

smed.h:	smed.x
	$(RPCGEN) -C -h smed.x > $@

smed_xdr.c: smed.x smed.h
	$(RPCGEN) -c smed.x > $@

smed_svc.c: smed.x
	$(RPCGEN) -m smed.x > $@

