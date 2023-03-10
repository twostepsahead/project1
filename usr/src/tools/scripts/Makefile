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
# Copyright (c) 1999, 2010, Oracle and/or its affiliates. All rights reserved.
#
# Copyright 2010, Richard Lowe

SHELL=/bin/sh

SHFILES= \
	Install \
	checkpaths \
	cstyle \
	elfcmp \
	genoffsets \
	protocmp.terse \

PERLFILES= \
	check_rtime \
	find_elf \
	interface_check \
	interface_cmp \
	validate_paths

PERLMODULES= \
	onbld_elfmod.pm \
	onbld_elfmod_vertype.pm


PYFILES= \
	cddlchk \
	copyrightchk \
	git-pbchk \
	hdrchk \
	mapfilechk \
	validate_pkg \
	wscheck \
	wsdiff

SCRIPTLINKS= \
	git-nits

MAN1ONBLDFILES= \
	Install.1 \
	cddlchk.1 \
	checkpaths.1 \
	check_rtime.1 \
	cstyle.1 \
	find_elf.1 \
	git-pbchk.1 \
	hdrchk.1 \
	interface_check.1 \
	interface_cmp.1 \
	mapfilechk.1 \
	wsdiff.1

MAN1ONBLDLINKS= \
	git-nits.1

ETCFILES= \
	its.conf \
	its.reg

EXCEPTFILES= \
	check_rtime \
	interface_check \
	interface_cmp

CLEANFILES = $(SHFILES) $(PERLFILES) $(PYFILES)

include ../Makefile.tools

ROOTONBLDSCRIPTLINKS = $(SCRIPTLINKS:%=$(ROOTONBLDBIN)/%)
ROOTONBLDMAN1ONBLDLINKS = $(MAN1ONBLDLINKS:%=$(ROOTONBLDMAN1ONBLD)/%)

$(ROOTONBLDETCFILES)		:= FILEMODE=	644
$(ROOTONBLDEXCEPTFILES)		:= FILEMODE=	644
$(ROOTONBLDPERLMODULES)		:= FILEMODE=	644
$(ROOTONBLDMAKEFILES)		:= FILEMODE=	644
$(ROOTONBLDMAN1ONBLDFILES)	:= FILEMODE=	644

.KEEP_STATE:

all:	$(SHFILES) $(PERLFILES) $(PERLMODULES) $(PYFILES) \
	$(MAN1ONBLDFILES)

$(ROOTONBLDBIN)/git-nits:
	$(RM) $(ROOTONBLDBIN)/git-nits
	$(SYMLINK) git-pbchk $(ROOTONBLDBIN)/git-nits

$(ROOTONBLDMAN1ONBLD)/git-nits.1:
	$(RM) $(ROOTONBLDMAN1ONBLD)/git-nits.1
	$(SYMLINK) git-pbchk.1 $(ROOTONBLDMAN1ONBLD)/git-nits.1

install: all .WAIT $(ROOTONBLDSHFILES) $(ROOTONBLDPERLFILES)		\
		$(ROOTONBLDPERLMODULES) $(ROOTONBLDPYFILES)		\
		$(ROOTONBLDSCRIPTLINKS) $(ROOTONBLDMAN1ONBLDFILES)	\
		$(ROOTONBLDMAKEFILES) $(ROOTONBLDETCFILES)		\
		$(ROOTONBLDEXCEPTFILES) $(ROOTONBLDMAN1ONBLDLINKS)

clean:
	$(RM) $(CLEANFILES)

bldenv.1: bldenv
	$(RM) "$@"
	(set +o errexit ; sh $? --nroff ; true) 2>&1 | \
	sed -e 's/\.DS/.nf/g;s/\.DE/.fi/' \
	-e 's/\.TH BLDENV 1/.TH BLDENV 1ONBLD/' \
	-e 's/(1)/(1ONBLD)/' > "$@"

include ../Makefile.targ

