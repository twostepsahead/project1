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
# Copyright 2009 Sun Microsystems, Inc.  All rights reserved.
# Use is subject to license terms.
#
# Copyright (c) 2018, Joyent, Inc.

DAEMON_SRCS =	powerd.c sysstat.c
DAEMON_OBJS =	$(DAEMON_SRCS:%.c=%.o)
DAEMON =	powerd
PMCFG_SRCS = 	conf.c parse.c handlers.c
PMCFG_OBJS =	$(PMCFG_SRCS:%.c=%.o)
PMCFG = 	pmconfig
SUSPEND_SRCS = 	sys-suspend.c pm_pam_conv.c
SUSPEND_OBJS =	$(SUSPEND_SRCS:%.c=%.o)
SUSPEND = 	sys-suspend
SRCS = 		$(DAEMON_SRCS) $(PMCFG_SRCS) $(SUSPEND_SRCS)
OBJS = 		$(SRCS:%.c=%.o)
PROG =		$(DAEMON) $(PMCFG) $(SUSPEND)
POWERCONF=	power.conf
ETCFILES =	$(POWERCONF)
POWERPERM =	power
DEFAULTFILES =	power.dfl

MANIFEST=	power.xml
SVCMETHOD=	svc-power

include ../Makefile.cmd

ROOTMANIFESTDIR =	$(ROOTSVCSYSTEM)

TEXT_DOMAIN=	SUNW_OST_OSCMD

XGETFLAGS +=    -a -x power_all.xcl
POFILE=		power_all.po
POFILES=	$(PMCFG_SRCS:%.c=%.po) $(SUSPEND_SRCS:%.c=%.po)

# pmconfig only needs libdevinfo on sparc
sparc_LDEVINFO=	-ldevinfo -lefi -ladm -lzfs -lnvpair
i386_LDEVINFO=

LDEVINFO=	-ldevinfo

DAEMON_LDLIBS =	$(LDLIBS.cmd) -lkstat $(LDEVINFO)
PMCFG_LDLIBS =	$(LDLIBS.cmd) -lsmbios -lkstat $($(MACH)_LDEVINFO)
SUSPEND_LDLIBS = $(LDLIBS.cmd) -lpam -lsecdb

ROOTUSRSBINPMCFG= $(PMCFG:%=$(ROOTUSRSBIN)/%)
$(ROOTUSRSBINPMCFG) := FILEMODE= 4555
ROOTUSRBINSUSPEND= $(SUSPEND:%=$(ROOTBIN)/%)
$(ROOTUSRBINSUSPEND) := FILEMODE= 4555

ROOTLIBPOWER= $(ROOTLIB64)/power
ROOTLIBPOWERDAEMON= $(DAEMON:%=$(ROOTLIBPOWER)/%)
$(ROOTLIBPOWER) := FILEMODE= 755
$(ROOTLIBPOWERDAEMON) := FILEMODE= 555

ROOTETCFILES= $(ETCFILES:%=$(ROOTETC)/%)
$(ROOTETCFILES) := FILEMODE= 644

SMOFF += signed

.PARALLEL: $(OBJS)

.KEEP_STATE:

all: $(PROG) $(POWERPERM).dfl $(ETCFILES)

install clean:

$(POWERCONF): $(POWERCONF).$(MACH)

$(DAEMON_OBJS): $(DAEMON_SRCS)
	$(CC) $(CFLAGS) $(CPPFLAGS) -o $@ -c $<

$(DAEMON): $(DAEMON_OBJS)
	$(CC) -o $@ $(CFLAGS) $(DAEMON_OBJS) $(LDFLAGS) $(DAEMON_LDLIBS)
	$(POST_PROCESS)

$(PMCFG_OBJS): pmconfig.h

$(PMCFG): $(PMCFG_OBJS)
	$(LINK.c) -o $@ $(PMCFG_OBJS) $(PMCFG_LDLIBS)
	$(POST_PROCESS)

$(SUSPEND): $(SUSPEND_OBJS)
	$(LINK.c) -o $@ $(SUSPEND_OBJS) $(SUSPEND_LDLIBS)
	$(POST_PROCESS)

install: all $(ROOTUSRSBINPMCFG) $(ROOTLIBPOWERDAEMON) $(ROOTUSRBINSUSPEND) \
	$(ROOTETCFILES) $(ROOTETCDEFAULTFILES) \
	$(ROOTMANIFEST) $(ROOTSVCMETHOD)

$(ROOTLIBPOWER):
	$(INS.dir)

$(ROOTLIBPOWER)/%:	%
	$(INS.file)

$(ROOTLIBPOWERDAEMON): $(ROOTLIBPOWER)

$(POFILE):	$(POFILES)
	$(RM)	$@
	cat	$(POFILES)	> $@

check:	$(CHKMANIFEST)

clean:
	$(RM) $(OBJS) $(POWERCONF)
	$(RM) $(POFILE) $(POFILES)

cstyle:
	$(CSTYLE) $(SRCS)

%:	%.$(MACH)
	$(RM) $@
	cat $< > $@

include ../Makefile.targ
