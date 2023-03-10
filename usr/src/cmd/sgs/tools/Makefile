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
# Copyright 2008 Sun Microsystems, Inc.  All rights reserved.
# Use is subject to license terms.
#
# ident	"%Z%%M%	%I%	%E% SMI"
#
# Makefile to support tools used for linker development.  GEXECS are those
# tools that the installed for distribution in the SUNWonld package.
#
#  o	`c' programs are built from the $(MACH)/Makefile
#
#  o	info: summarizes relocation overhead, and a sorted string listing.
#
#  o	proto: creates a proto directory structure suitable for sgs make
#	installs (basically a subset of Targetdirs).
#
# cmd/sgs/tools/Makefile

include		$(SRC)/Makefile.master

LSSRCS=		info.sh	\
		proto.sh

SRCS=		$(LSSRCS)

LEXECS=		$(LSSRCS:%.sh=%)

ROOTDIR =	$(ROOT)/opt/SUNWonld/bin
ROOTPROGS =	$(GEXECS:%=$(ROOTDIR)/%)
ROOTPROGS +=	$(PSRCS:%=$(ROOTDIR)/%)

$(ROOTPROGS) :=	FILEMODE=755

SUBDIRS=	$(MACH) man

all:=		TARGET= all
install:=	TARGET= install
clean:=		TARGET= clean
clobber:=	TARGET= clobber


.KEEP_STATE:

all:		$(EXECS) $(SUBDIRS)

install:	all $(ROOTPROGS) $(SUBDIRS)

clean clobber:	$(SUBDIRS)
		$(RM) $(RMFLAGS) $(EXECS)

delete:

%:		%.ksh
		$(RM) $@
		cat $< > $@
		chmod +x $@

$(SUBDIRS):	FRC
		@cd $@; pwd; $(MAKE) $(TARGET)

$(ROOTDIR)/%:	%
		$(INS.file)

$(ROOTDIR)/%:	scoping/%
		$(INS.file)

FRC:

execs:		$(EXECS)
