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
# Copyright 2010 Sun Microsystems, Inc.  All rights reserved.
# Use is subject to license terms.
#
# Copyright (c) 2018, Joyent, Inc.

#
#	Path to the base of the uts directory tree (usually /usr/src/uts).
#
UTSBASE	= ../..
COM_DIR = $(COMMONBASE)/crypto

#
#	Define the module and object file sets.
#
MODULE		= kcf
KCF_OBJS_32	=
KCF_OBJS_64	= gcm_intel.o
KCF_OBJS	+= $(KCF_OBJS_$(CLASS))
OBJECTS		= $(KCF_OBJS:%=$(OBJS_DIR)/%)
ROOTMODULE	= $(ROOT_MISC_DIR)/$(MODULE)

#
#	Include common rules.
#
include $(UTSBASE)/intel/Makefile.intel

#
#	Define targets
#
ALL_TARGET	= $(BINARY)
INSTALL_TARGET	= $(BINARY) $(ROOTMODULE)

#
# Linkage dependencies
#

CFLAGS		+= -I$(COM_DIR)
AS_CPPFLAGS	+= -I$(SRCTOP)/arch/x86/kernel/include


CERRWARN	+= -Wno-switch
CERRWARN	+= -Wno-uninitialized
CERRWARN	+= -Wno-parentheses
CERRWARN	+= -Wno-unused-label

# needs work
SMOFF += all_func_returns,signed_integer_overflow_check

#
#	Default build targets.
#
.KEEP_STATE:

def:		$(DEF_DEPS)

all:		$(ALL_DEPS)

clean:		$(CLEAN_DEPS)

clobber:	$(CLOBBER_DEPS)

install:	$(INSTALL_DEPS)

#
#	Include common targets.
#
include $(UTSBASE)/intel/Makefile.targ

$(OBJS_DIR)/%.o: $(COM_DIR)/modes/amd64/%.s
	$(COMPILE64.s) -o $@ $(COM_DIR)/modes/amd64/${@F:.o=.s}
	$(POST_PROCESS_O)
