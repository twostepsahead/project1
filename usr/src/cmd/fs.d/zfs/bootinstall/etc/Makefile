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

.PARALLEL:

include $(SRC)/cmd/Makefile.cmd

CONF = SUNW,EC_zfs,ESC_ZFS_bootfs_vdev_attach,sysevent.conf
CONFDIR = $(ROOTETC)/sysevent/config

# utilize the predefined install target patterns in cmd/Makefile.targ by
# overriding ROOTCMDDIR
#
ROOTCMDDIR = $(CONFDIR)
FILEMODE = 0644

.KEEP_STATE:

all clean clobber:

install: $(CONF:%=$(CONFDIR)/%)

include $(SRC)/cmd/Makefile.targ
