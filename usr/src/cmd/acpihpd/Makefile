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

#
# Copyright (c) 2010, Intel Corporation.
# All rights reserved.
#

#
# cmd/acpihpd/Makefile
#

MANIFEST = acpihpd.xml
SVCMETHOD = svc-acpihpd

include ../Makefile.cmd

$(64ONLY)SUBDIRS= $(MACH)

ROOTMANIFESTDIR = $(ROOTSVCPLATFORM)

all	:=	TARGET = all
install	:=	TARGET = install
clean	:=	TARGET = clean
clobber	:=	TARGET = clobber

.KEEP_STATE:

CLOBBERFILES += THIRDPARTYLICENSE

all:	$(SUBDIRS)

install:	$(SUBDIRS) $(ROOTMANIFEST) $(ROOTSVCMETHOD)

all install: THIRDPARTYLICENSE

THIRDPARTYLICENSE: acpihpd.c
	$(SED) -e '/sys\/param.h/,$$ d' acpihpd.c > $@

clean clobber:	$(SUBDIRS)

check:	$(CHKMANIFEST)

$(SUBDIRS):	FRC
	@cd $@; pwd; $(MAKE) $(TARGET)

FRC:

include ../Makefile.targ
