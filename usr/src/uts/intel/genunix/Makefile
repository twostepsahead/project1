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

#
#	This makefile drives the production of the generic
#	unix kernel module.
#
#	x86 implementation architecture dependent
#

#
#	Path to the base of the uts directory tree (usually /usr/src/uts).
#
UTSBASE	= ../..

#
#	Define the module and object file sets.
#
MODULE		= genunix
GENUNIX		= $(OBJS_DIR)/$(MODULE)

OBJECTS		= $(GENUNIX_OBJS:%=$(OBJS_DIR)/%) \
		  $(NOT_YET_KMODS:%=$(OBJS_DIR)/%)

ROOTMODULE	= $(ROOT_KERN_DIR)/$(MODULE)

LIBGEN		= $(OBJS_DIR)/libgenunix.so
LIBSTUBS	= $(GENSTUBS_OBJS:%=$(OBJS_DIR)/%)

#
#	Include common rules.
#
include $(UTSBASE)/intel/Makefile.intel

#
#	Define targets
#
ALL_TARGET	= $(LIBGEN) $(GENUNIX)
INSTALL_TARGET	= $(LIBGEN) $(GENUNIX) $(ROOTMODULE)

#
#	Overrides
#
CLOBBERFILES	+= $(GENUNIX)
CLEANFILES	+= $(LIBSTUBS) $(LIBGEN)
BINARY		=

CPPFLAGS	+= -I$(SRCTOP)/include
CPPFLAGS	+= -I$(SRC)/common
CPPFLAGS	+= -I$(SRCTOP)/kernel/fs
CPPFLAGS	+= -I$(SRCTOP)/kernel/fs/zfs
CPPFLAGS	+= -I$(SRCTOP)/arch/x86/kernel/include

#
# For now, disable these compiler warnings; maintainers should endeavor to
# investigate and remove these for maximum coverage.  Please do not carry
# these forward to new Makefiles.
#

CERRWARN	+= -Wno-unused-label
CERRWARN	+= -Wno-unused-variable
CERRWARN	+= -Wno-unused-value
CERRWARN	+= -Wno-unused-function
CERRWARN	+= -Wno-parentheses
CERRWARN	+= -Wno-switch
CERRWARN	+= -Wno-type-limits
CERRWARN	+= -Wno-uninitialized
CERRWARN	+= -Wno-clobbered
CERRWARN	+= -Wno-empty-body

# false positives
SMOFF += index_overflow
$(OBJS_DIR)/seg_vn.o := SMOFF += deref_check
$(OBJS_DIR)/ddi_intr_irm.o := SMOFF += deref_check

# need work still
SMOFF += signed,indenting,all_func_returns
$(OBJS_DIR)/clock_highres.o := SMOFF += signed_integer_overflow_check
$(OBJS_DIR)/evchannels.o := SMOFF += allocating_enough_data
$(OBJS_DIR)/klpd.o := SMOFF += cast_assign
$(OBJS_DIR)/lookup.o := SMOFF += strcpy_overflow
$(OBJS_DIR)/process.o := SMOFF += or_vs_and
$(OBJS_DIR)/sunpci.o := SMOFF += deref_check
$(OBJS_DIR)/timers.o := SMOFF += signed_integer_overflow_check

# definitely wrong
$(OBJS_DIR)/acl_common.o := SMOFF += or_vs_and

#
#	Default build targets.
#
.KEEP_STATE:

def:		$(DEF_DEPS)

all:		$(ALL_DEPS)

clean:		$(CLEAN_DEPS)

clobber:	$(CLOBBER_DEPS)

install:	$(INSTALL_DEPS)

$(LIBGEN):	$(GENUNIX) $(LIBSTUBS)
	$(BUILD.SO) $(GENUNIX) $(LIBSTUBS)

$(GENUNIX): $(OBJECTS)
	$(LD) -r $(LDFLAGS) -o $@ $(OBJECTS)
	$(CTFMERGE_MODULE)
	$(POST_PROCESS)

#
#	Include common targets.
#
include $(UTSBASE)/intel/Makefile.targ

#
#	Software workarounds for hardware "features".
#
include	$(UTSBASE)/i86pc/Makefile.workarounds

ALL_DEFS += $(WORKAROUND_DEFS)
