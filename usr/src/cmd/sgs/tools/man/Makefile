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
#ident	"%Z%%M%	%I%	%E% SMI"
#
# Copyright 2003 Sun Microsystems, Inc.  All rights reserved.
# Use is subject to license terms.
#
# Note, these man pages are not part of the product.

MAN1l=		chkmsg.1l \
		sgsmsg.1l

MANS=		$(MAN1l)

include 	../../../../Makefile.master

ROOTDIR=	$(ROOT)/opt/SUNWonld/man
ROOT1lMAN=	$(MAN1l:%=$(ROOTDIR)/man1l/%)

ROOTMANS=	$(ROOT1lMAN)

FILEMODE=	0444

.KEEP_STATE:
all:		$(MANS)

install:	all $(ROOTMANS)

$(ROOTDIR)/man1l/%:	%
		$(INS.file)

clobber clean:
