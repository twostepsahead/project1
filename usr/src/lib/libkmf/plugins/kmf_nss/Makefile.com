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
# Copyright (c) 2018, Joyent, Inc.

LIBRARY=	kmf_nss.a
VERS=		.1

OBJECTS=	nss_spi.o

include	$(SRC)/lib/Makefile.lib

MPSDIR=		/usr/lib/mps
KMFINC=		-I../../../include -I../../../ber_der/inc
NSSINC=		-I$(ADJUNCT_PROTO)/usr/include/mps
BERLIB=		-lkmf -lkmfberder
BERLIB64=	$(BERLIB)

NSSLIBS=	$(BERLIB) -L$(ADJUNCT_PROTO)$(MPSDIR) -R$(MPSDIR) \
		-lnss3 -lnspr4 -lsmime3 -lc
NSSLIBS64=	$(BERLIB64) -L$(ADJUNCT_PROTO)$(MPSDIR)/$(MACH64) \
		-R$(MPSDIR)/$(MACH64) -lnss3 -lnspr4 -lsmime3 -lc

SRCDIR=		../common
INCDIR=		../../include

CPPFLAGS	+=	$(KMFINC) $(NSSINC)  \
		-I$(INCDIR) -I$(ADJUNCT_PROTO)/usr/include/libxml2

PICS=	$(OBJECTS:%=pics/%)

CERRWARN	+=	-Wno-unused-label
CERRWARN	+=	-Wno-unused-value
CERRWARN	+=	-Wno-uninitialized

# not linted
SMATCH=off

LDLIBS32	+=	$(NSSLIBS)

LIBS	=	$(DYNLIB)

ROOTLIBDIR=	$(ROOT)/lib/crypto/$(MACH32)
ROOTLIBDIR64=	$(ROOT)/lib/crypto

.KEEP_STATE:

all:	$(LIBS)

FRC:

include $(SRC)/lib/Makefile.targ
