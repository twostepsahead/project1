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
# Copyright 2014 Garrett D'Amore <garrett@damore.org>
# Copyright (c) 1989, 2010, Oracle and/or its affiliates. All rights reserved.
#
# Definitions common to command source.
#
# include global definitions; SRC should be defined in the shell.
# SRC is needed until RFE 1026993 is implemented.

include $(SRC)/Makefile.master

LN=		ln
SH=		sh
ECHO=		echo
MKDIR=		mkdir
TOUCH=		touch

FILEMODE=	0555
LIBFILEMODE=	0444

KRB5DIR=	$(ROOT)/usr
KRB5BIN=	$(KRB5DIR)/bin
KRB5SBIN=	$(KRB5DIR)/sbin
KRB5LIB=	$(KRB5DIR)/lib/krb5
KRB5RUNPATH=	/usr/lib/krb5

ROOTBIN=		$(ROOT)/usr/bin
ROOTLIB=		$(ROOT)/usr/lib/$(MACH32)
ROOTLIBEXEC=		$(ROOT)/usr/libexec
ROOTLIBEXEC32=		$(ROOT)/usr/libexec/$(MACH32)
ROOTLIBSVCBIN=		$(ROOT)/lib/svc/bin
ROOTLIBSVCMETHOD=	$(ROOT)/lib/svc/method
ROOTLIBZONES=		$(ROOT)/lib/zones

ROOTSHLIB=	$(ROOT)/usr/share/lib

ROOTSHLIBCCS=	$(ROOTSHLIB)/ccs
ROOTSBIN=	$(ROOT)/sbin
ROOTUSRSBIN=	$(ROOT)/usr/sbin
ROOTETC=	$(ROOT)/etc

ROOTETCSECURITY=	$(ROOTETC)/security
ROOTETCSECLIB=	$(ROOTETCSECURITY)/lib
ROOTETCZONES=	$(ROOTETC)/zones

ROOTETCFTPD=	$(ROOT)/etc/ftpd
ROOTETCINET=	$(ROOT)/etc/inet
ROOTBIN32=	$(ROOTBIN)/$(MACH32)
ROOTBIN64=	$(ROOTBIN)/$(MACH64)
ROOTCMDDIR64=	$(ROOTCMDDIR)/$(MACH64)
ROOTLIB64=	$(ROOT)/usr/lib
ROOTUSRSBIN32=	$(ROOTUSRSBIN)/$(MACH32)
ROOTUSRSBIN64=	$(ROOTUSRSBIN)/$(MACH64)
ROOTVARSMB=	$(ROOT)/var/smb


#
# Like ROOTLIBDIR in $(SRC)/Makefile.lib, any lower-level Makefiles that
# put their binaries in a non-standard location should reset this and use
# $(ROOTCMD) in their `install' target. By default we set this to a bogus
# value so that it will not conflict with any of the other values already
# defined in this Makefile.
#
ROOTCMDDIR=	$(ROOT)/__nonexistent_directory__

ISAEXEC=	$(ROOT)/usr/lib/isaexec
PLATEXEC=	$(ROOT)/usr/lib/platexec

LDLIBS =	$(LDLIBS.cmd)

LDFLAGS.cmd = \
	$(BDIRECT) $(ENVLDFLAGS1) $(ENVLDFLAGS2) $(ENVLDFLAGS3) \
	$(MAPFILE.PGA:%=-Wl,-M%)

LDFLAGS =	$(LDFLAGS.cmd)


KRB5PROG=	$(PROG:%=$(KRB5BIN)/%)
KRB5SBINPROG=	$(PROG:%=$(KRB5SBIN)/%)
KRB5LIBPROG=	$(PROG:%=$(KRB5LIB)/%)

ROOTPROG=	$(PROG:%=$(ROOTBIN)/%)
ROOTCMD=	$(PROG:%=$(ROOTCMDDIR)/%)
ROOTSHFILES=	$(SHFILES:%=$(ROOTBIN)/%)
ROOTLIBPROG=	$(PROG:%=$(ROOTLIB64)/%)
ROOTLIBSHFILES= $(SHFILES:%=$(ROOTLIB64)/%)
ROOTSHLIBPROG=	$(PROG:%=$(ROOTSHLIB)/%)
ROOTSBINPROG=	$(PROG:%=$(ROOTSBIN)/%)
ROOTUSRSBINPROG=$(PROG:%=$(ROOTUSRSBIN)/%)
ROOTUSRSBINSCRIPT=$(SCRIPT:%=$(ROOTUSRSBIN)/%)
ROOTLOCALEPROG=	$(PROG:%=$(ROOTLOCALEDEF)/%)
ROOTPROG64=	$(PROG:%=$(ROOTBIN64)/%)
ROOTPROG32=	$(PROG:%=$(ROOTBIN32)/%)
ROOTCMD64=	$(PROG:%=$(ROOTCMDDIR64)/%)
ROOTUSRSBINPROG32=	$(PROG:%=$(ROOTUSRSBIN32)/%)
ROOTUSRSBINPROG64=	$(PROG:%=$(ROOTUSRSBIN64)/%)

ROOTETCDEFAULT=	$(ROOTETC)/default
ROOTETCDEFAULTFILES=	$(DEFAULTFILES:%.dfl=$(ROOTETCDEFAULT)/%)
$(ROOTETCDEFAULTFILES) :=	FILEMODE = 0644

ROOTETCSECFILES=	$(ETCSECFILES:%=$(ROOTETCSECURITY)/%)
$(ROOTETCSECFILES) :=	FILEMODE = 0644

ROOTETCSECLIBFILES=	$(ETCSECLIBFILES:%=$(ROOTETCSECLIB)/%)

ROOTETCZONESFILES=	$(ETCZONESFILES:%=$(ROOTETCZONES)/%)
$(ROOTETCZONESFILES) :=	FILEMODE = 0444

ROOTLIBZONESFILES=	$(LIBZONESFILES:%=$(ROOTLIBZONES)/%)
$(ROOTLIBZONESFILES) :=	FILEMODE = 0555

#
# Directories for smf(5) service manifests and profiles.
#
ROOTSVC=			$(ROOT)/lib/svc
ROOTETCSVC=			$(ROOT)/etc/svc

ROOTSVCMANIFEST=		$(ROOTSVC)/manifest
ROOTSVCPROFILE=			$(ROOTETCSVC)/profile

ROOTSVCMILESTONE=		$(ROOTSVCMANIFEST)/milestone
ROOTSVCDEVICE=			$(ROOTSVCMANIFEST)/device
ROOTSVCSYSTEM=			$(ROOTSVCMANIFEST)/system
ROOTSVCSYSTEMDEVICE=		$(ROOTSVCSYSTEM)/device
ROOTSVCSYSTEMFILESYSTEM=	$(ROOTSVCSYSTEM)/filesystem
ROOTSVCSYSTEMSECURITY=		$(ROOTSVCSYSTEM)/security
ROOTSVCNETWORK=			$(ROOTSVCMANIFEST)/network
ROOTSVCNETWORKDNS=		$(ROOTSVCNETWORK)/dns
ROOTSVCNETWORKISCSI=		$(ROOTSVCNETWORK)/iscsi
ROOTSVCNETWORKLDAP=		$(ROOTSVCNETWORK)/ldap
ROOTSVCNETWORKNFS=		$(ROOTSVCNETWORK)/nfs
ROOTSVCNETWORKNIS=		$(ROOTSVCNETWORK)/nis
ROOTSVCNETWORKROUTING=		$(ROOTSVCNETWORK)/routing
ROOTSVCNETWORKRPC=		$(ROOTSVCNETWORK)/rpc
ROOTSVCNETWORKSMB=		$(ROOTSVCNETWORK)/smb
ROOTSVCNETWORKSECURITY=		$(ROOTSVCNETWORK)/security
ROOTSVCNETWORKIPSEC=		$(ROOTSVCNETWORK)/ipsec
ROOTSVCNETWORKSHARES=		$(ROOTSVCNETWORK)/shares
ROOTSVCSMB=			$(ROOTSVCNETWORK)/smb
ROOTSVCPLATFORM=		$(ROOTSVCMANIFEST)/platform
ROOTSVCPLATFORMSUN4U=		$(ROOTSVCPLATFORM)/sun4u
ROOTSVCPLATFORMSUN4V=		$(ROOTSVCPLATFORM)/sun4v
ROOTSVCAPPLICATION=		$(ROOTSVCMANIFEST)/application
ROOTSVCAPPLICATIONMANAGEMENT=	$(ROOTSVCAPPLICATION)/management
ROOTSVCAPPLICATIONSECURITY=	$(ROOTSVCAPPLICATION)/security
ROOTSVCAPPLICATIONPRINT=	$(ROOTSVCAPPLICATION)/print

#
# Commands Makefiles delivering a manifest are expected to define MANIFEST.
#
# Like ROOTCMDDIR, any lower-level Makefiles that put their manifests in a
# subdirectory of the manifest directories listed above should reset
# ROOTMANIFESTDIR and use it in their `install' target. By default we set this
# to a bogus value so that it will not conflict with any of the other values
# already  defined in this Makefile.
#
# The manifest validation of the $SRC/cmd check target is also derived from a
# valid MANIFEST setting.
#
ROOTMANIFESTDIR=	$(ROOTSVCMANIFEST)/__nonexistent_directory__
ROOTMANIFEST=		$(MANIFEST:%=$(ROOTMANIFESTDIR)/%)
CHKMANIFEST=		$(MANIFEST:%.xml=%.xmlchk)

# Manifests cannot be checked in parallel, because we are using the global
# repository that is in $(SRC)/cmd/svc/seed/global.db.  This is a
# repository that is built from the manifests in this workspace, whereas
# the build machine's repository may be out of sync with these manifests.
# Because we are using a private repository, svccfg-native must start up a
# private copy of configd-native.  We cannot have multiple copies of
# configd-native trying to access global.db simultaneously.

.NO_PARALLEL:	$(CHKMANIFEST)

#
# For installing "starter scripts" of services
#

ROOTSVCMETHOD=		$(SVCMETHOD:%=$(ROOTLIBSVCMETHOD)/%)

ROOTSVCBINDIR=		$(ROOTLIBSVCBIN)/__nonexistent_directory__
ROOTSVCBIN= 		$(SVCBIN:%=$(ROOTSVCBINDIR)/%)

#

# For programs that are installed in the root filesystem,
# build $(ROOTFS_PROG) rather than $(PROG)
$(ROOTFS_PROG) := LDFLAGS += -Wl,-I/lib/ld.so.1

$(KRB5BIN)/%: %
	$(INS.file)

$(KRB5SBIN)/%: %
	$(INS.file)

$(KRB5LIB)/%: %
	$(INS.file)

$(ROOTBIN)/%: %
	$(INS.file)

$(ROOTLIB)/%: %
	$(INS.file)

$(ROOTBIN64)/%: %
	$(INS.file)

$(ROOTLIB64)/%: %
	$(INS.file)

$(ROOTBIN32)/%: %
	$(INS.file)

$(ROOTSHLIB)/%: %
	$(INS.file)

$(ROOTSBIN)/%: %
	$(INS.file)

$(ROOTUSRSBIN)/%: %
	$(INS.file)

$(ROOTUSRSBIN32)/%: %
	$(INS.file)

$(ROOTUSRSBIN64)/%: %
	$(INS.file)

$(ROOTETC)/%: %
	$(INS.file)

$(ROOTETCFTPD)/%: %
	$(INS.file)

$(ROOTETCINET)/%: %
	$(INS.file)

$(ROOTETCDEFAULT)/%:	%.dfl
	$(INS.rename)

$(ROOTETCSECLIB)/%: %
	$(INS.file)

$(ROOTETCZONES)/%: %
	$(INS.file)

$(ROOTLIBZONES)/%: %
	$(INS.file)

$(ROOTLOCALEDEF)/%: %
	$(INS.file)

$(ROOTCHARMAP)/%: %
	$(INS.file)

$(ROOTI18NEXT)/%: %
	$(INS.file)

$(ROOTI18NEXT64)/%: %
	$(INS.file)

$(ROOTLIBSVCMETHOD)/%: %
	$(INS.file)

$(ROOTLIBSVCBIN)/%: %
	$(INS.file)

$(ROOTSVCMILESTONE)/%: %
	$(INS.file)

$(ROOTSVCDEVICE)/%: %
	$(INS.file)

$(ROOTSVCSYSTEM)/%: %
	$(INS.file)

$(ROOTSVCSYSTEMDEVICE)/%: %
	$(INS.file)

$(ROOTSVCSYSTEMFILESYSTEM)/%: %
	$(INS.file)

$(ROOTSVCSYSTEMSECURITY)/%: %
	$(INS.file)

$(ROOTSVCNETWORK)/%: %
	$(INS.file)

$(ROOTSVCNETWORKLDAP)/%: %
	$(INS.file)

$(ROOTSVCNETWORKNFS)/%: %
	$(INS.file)

$(ROOTSVCNETWORKNIS)/%: %
	$(INS.file)

$(ROOTSVCNETWORKRPC)/%: %
	$(INS.file)

$(ROOTSVCNETWORKSECURITY)/%: %
	$(INS.file)

$(ROOTSVCNETWORKIPSEC)/%: %
	$(INS.file)

$(ROOTSVCNETWORKSHARES)/%: %
	$(INS.file)

$(ROOTSVCNETWORKSMB)/%: %
	$(INS.file)

$(ROOTSVCAPPLICATION)/%: %
	$(INS.file)

$(ROOTSVCAPPLICATIONMANAGEMENT)/%: %
	$(INS.file)

$(ROOTSVCAPPLICATIONSECURITY)/%: %
	$(INS.file)

$(ROOTSVCAPPLICATIONPRINT)/%: %
	$(INS.file)

$(ROOTSVCPLATFORM)/%: %
	$(INS.file)

$(ROOTSVCPLATFORMSUN4U)/%: %
	$(INS.file)

$(ROOTSVCPLATFORMSUN4V)/%: %
	$(INS.file)

# Install rule for gprof, yacc, and lex dependency files
$(ROOTSHLIBCCS)/%: ../common/%
	$(INS.file)

$(ROOTVARSMB)/%: %
	$(INS.file)

# build rule for statically linked programs with single source file.
%.static: %.c
	$(LINK.c) -o $@ $< $(LDLIBS)
	$(POST_PROCESS)

# Define the majority text domain in this directory.
TEXT_DOMAIN= SUNW_OST_OSCMD	

CLOBBERFILES += $(DCFILE)

# This flag is for programs which should not build a 32-bit binary
sparc_64ONLY= $(POUND_SIGN)
64ONLY=	 $($(MACH)_64ONLY)
