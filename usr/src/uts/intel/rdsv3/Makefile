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
# Copyright (c) 2010, Oracle and/or its affiliates. All rights reserved.
#
# Copyright (c) 2018, Joyent, Inc.

#
#	Path to the base of the uts directory tree (usually /usr/src/uts).
#
UTSBASE	= ../..

#
#	Define the module and object file sets.
#
MODULE		= rdsv3
OBJECTS		= $(RDSV3_OBJS:%=$(OBJS_DIR)/%)
ROOTMODULE	= $(ROOT_DRV_DIR)/$(MODULE)
CFLAGS 		+= $(_XPG4_2)
LDFLAGS		+= -dy -Nfs/sockfs -Nmisc/ksocket -Ndrv/ip -Nmisc/ibtl -Nmisc/ibcm -Nmisc/sol_ofs
CONF_SRCDIR	= $(UTSBASE)/common/io/ib/clients/rdsv3
#
#	Include common rules.
#
include $(UTSBASE)/intel/Makefile.intel

# CFLAGS 		+= -DOFA_SOLARIS

CERRWARN	+= -Wno-unused-label
CERRWARN	+= -Wno-unused-variable
CERRWARN	+= -Wno-unused-function
CERRWARN	+= -Wno-uninitialized
CERRWARN	+= -Wno-parentheses

# needs work
SMATCH=off

#
#	Define targets
#
ALL_TARGET	= $(BINARY) $(SRC_CONFFILE)
INSTALL_TARGET	= $(BINARY) $(ROOTMODULE) $(ROOT_CONFFILE)

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

