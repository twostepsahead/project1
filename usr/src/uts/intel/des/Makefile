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
# uts/intel/des/Makefile
#
# Copyright 2014 Garrett D'Amore <garrett@damore.org>
#
# Copyright 2008 Sun Microsystems, Inc.  All rights reserved.
# Use is subject to license terms.
#

#
#	This makefile drives the production of the des crypto kernel module.
#
#	intel architecture dependent
#

#
#	Path to the base of the uts directory tree (usually /usr/src/uts).
#
UTSBASE	= ../..
COM_DIR = $(COMMONBASE)/crypto

#
#	Define the module and object file sets.
#
MODULE		= des
OBJECTS		= $(DES_OBJS:%=$(OBJS_DIR)/%)
ROOTMODULE	= $(ROOT_CRYPTO_DIR)/$(MODULE)
ROOTLINK	= $(ROOT_MISC_DIR)/$(MODULE)
LINK_TARGET	= ../../kernel/crypto/$(MODULE)

#
#	Include common rules.
#
include $(UTSBASE)/intel/Makefile.intel

#
#	Define targets
#
ALL_TARGET	= $(BINARY)
INSTALL_TARGET	= $(BINARY) $(ROOTMODULE) $(ROOTLINK)

#
# Linkage dependencies
#
LDFLAGS		+= -dy -Nmisc/kcf

CPPFLAGS	+= -I$(COM_DIR)

#
# For now, disable these compiler warnings; maintainers should endeavor to
# investigate and remove these for maximum coverage.  Please do not carry
# these forward to new Makefiles.
#

CERRWARN	+= -Wno-parentheses
CERRWARN	+= -Wno-uninitialized

#
#	Default build targets.
#
.KEEP_STATE:

def:		$(DEF_DEPS)

all:		$(ALL_DEPS)

clean:		$(CLEAN_DEPS)

clobber:	$(CLOBBER_DEPS)

install:	$(INSTALL_DEPS)

$(ROOTLINK):	$(ROOT_MISC_DIR) $(ROOTMODULE)
	-$(RM) $@; ln -s $(LINK_TARGET) $@

#
#	Include common targets.
#
include $(UTSBASE)/intel/Makefile.targ
