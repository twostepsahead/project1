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
# Copyright (c) 2004, 2010, Oracle and/or its affiliates. All rights reserved.
#

include		$(SRC)/Makefile.master

ROOTDIR=	$(ROOT)/usr/lib/ld/$(MACH32)

DIRS=		$(ROOTDIR)
FILES=

install: $(DIRS) $(FILES) map.link

$(FILES):=	FILEMODE=444

$(DIRS):=	DIRMODE=755

map.link:
	$(RM) $(ROOTDIR)/map.default \
		$(ROOTDIR)/map.above4G $(ROOTDIR)/map.below4G
	$(SYMLINK) ../map.default $(ROOTDIR)/map.default
	$(SYMLINK) ../map.above4G $(ROOTDIR)/map.above4G
	$(SYMLINK) ../map.below4G $(ROOTDIR)/map.below4G

$(DIRS):
	$(INS.dir)

$(ROOTDIR)/%: %
	$(INS.file)

$(ROOTDIR64)/%: %
	$(INS.file)
