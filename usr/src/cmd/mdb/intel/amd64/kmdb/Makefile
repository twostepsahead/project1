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

#
# Build and link kmdb
#
all: $(PROG) kmdb_modlinktest.o

%.o: ../../kmdb/%.c
	$(COMPILE.c) $<
	$(CTFCONVERT_O)

%.o: ../../mdb/%.c
	$(COMPILE.c) $<
	$(CTFCONVERT_O)

%.o: $(SRC)/common/dis/i386/%.c
	$(COMPILE.c) $<
	$(CTFCONVERT_O)

ROOT_MISC_DIR		= $(ROOT)/kernel/misc

ROOTMISC		= $(ROOT_MISC_DIR)/$(PROG)

include ../../../../Makefile.cmd
include ../../../../Makefile.cmd.64

include ../../Makefile.amd64
include ../../../Makefile.kmdb

STANDLIBS += $(ROOT)/usr/lib/libstanddisasm.so
STANDLIBS += $(ROOT)/usr/lib/libstandsaveargs.so

INCDIRS += $(SRCTOP)/arch/x86/kernel/include
INCDIRS += $(SRC)/common/dis/i386

CPPFLAGS += -DDIS_TEXT

install: all $(ROOTMISC)

kmdb_context_off.h: ../../kmdb/kmdb_context_off.in
	$(OFFSETS_CREATE) <../../kmdb/kmdb_context_off.in >$@

#
# Installation targets
#

$(ROOT_MISC_DIR):
	-$(INS.dir)

$(ROOT_MISC_DIR)/%: 	% $(ROOT_MISC_DIR)
	$(INS.file)
