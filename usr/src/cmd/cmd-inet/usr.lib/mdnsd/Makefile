
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
# Copyright 2007 Sun Microsystems, Inc.  All rights reserved.
# Use is subject to license terms.
# Copyright 2016 Toomas Soome <tsoome@me.com>
#
# Copyright (c) 2018, Joyent, Inc.

PROG=		mdnsd
MANIFEST=	multicast.xml

SRCDIR=		$(SRC)/contrib/mDNSResponder

OBJS=		DNSCommon.o DNSDigest.o GenLinkedList.o \
		PlatformCommon.o PosixDaemon.o \
		mDNS.o mDNSDebug.o mDNSPosix.o mDNSUNP.o \
		uDNS.o uds_daemon.o CryptoAlg.o anonymous.o dnssd_ipc.o
SRCS=		$(OBJS:%.o=%.c)

MDNSFLAGS=	-DNOT_HAVE_SA_LEN \
		-DLOG_PERROR=0 -DHAVE_SOLARIS -DTARGET_OS_SOLARIS \
		-DHAVE_BROKEN_RECVIF_NAME \
		-DHAVE_IPV6=1 -Dasm=__asm -DMDNSD_NOROOT \
		-DPID_FILE=\"\" -DMDNSD_USER=\"noaccess\" \
		-DMDNS_VERSIONSTR_NODTS

include	../../../Makefile.cmd

CERRWARN +=	-Wno-unused-variable
CERRWARN +=	-Wno-implicit-function-declaration
CERRWARN +=	-Wno-uninitialized

# not linted
SMATCH=off

ROOTMANIFESTDIR=	$(ROOTSVCNETWORKDNS)
$(ROOTMANIFEST)		:= FILEMODE= 444

.PARALLEL:      $(OBJS)
.WAIT:          $(PROG)
.KEEP_STATE:

all:	$(PROG)

$(PROG): $(OBJS)
	$(LINK.c) $(OBJS) -o $@ $(LDLIBS)
	$(POST_PROCESS)

include ../Makefile.lib

CSTD = $(CSTD_GNU99)
CPPFLAGS += $(MDNSFLAGS)
CPPFLAGS += -I$(SRCDIR)/mDNSShared -I$(SRCDIR)/mDNSPosix
CPPFLAGS += -I$(SRCDIR)/mDNSCore

install:	all $(ROOTLIBINETPROG) $(ROOTMANIFEST)

%.o:		$(SRCDIR)/mDNSCore/%.c
		$(COMPILE.c) $(OUTPUT_OPTION) $<
		$(POST_PROCESS_O)

%.o:		$(SRCDIR)/mDNSShared/%.c
		$(COMPILE.c) $(OUTPUT_OPTION) $<
		$(POST_PROCESS_O)

%.o:		$(SRCDIR)/mDNSPosix/%.c
		$(COMPILE.c) $(OUTPUT_OPTION) $<
		$(POST_PROCESS_O)

check:		$(CHKMANIFEST)

clean:
	$(RM) $(OBJS)

include ../../../Makefile.targ
