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
#ident	"%Z%%M%	%I%	%E% SMI"
#
# Copyright 2006 Sun Microsystems, Inc.  All rights reserved.
# Use is subject to license terms.
#

SUBDIR1=	fstyp
SUBDIR2=	fsck fsdb labelit mkfs mount
SUBDIRS=	$(SUBDIR1) $(SUBDIR2)

all:=		TARGET= all
install:=	TARGET= install
clean:=		TARGET= clean
clobber:=	TARGET= clobber
putback:=	TARGET= putback
catalog:=       TARGET= catalog

# for messaging catalog
#
POFILE= udfs.po
POFILES= $(SUBDIR2:%=%/%.po)

.KEEP_STATE:

.PARALLEL:	$(SUBDIRS)

all install:	$(SUBDIRS)

catalog:        $(POFILE)

$(POFILE):      $(SUBDIR2)
	$(RM) 	$@
	cat     $(POFILES)      > $@

clean clobber putback : $(SUBDIRS)

$(SUBDIRS): FRC
	@cd $@; pwd; $(MAKE) $(MFLAGS) $(TARGET)

FRC:
