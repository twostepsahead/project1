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

SBINPROG = keyserv newkey
BINPROG	= keylogout keylogin domainname chkey

MANIFEST= keyserv.xml

PROG= $(SBINPROG) $(BINPROG)

HDRS= key_prot.h

DEFAULTFILES = keyserv.dfl

KEYSERVOBJS = keyserv.o gen_dhkeys.o xcrypt.o
KEYLOGOUTOBJS = keylogout.o
KEYLOGINOBJS = keylogin.o
CHKEYOBJS = chkey.o gen_dhkeys.o xcrypt.o
NEWKEYOBJS = newkey.o gen_dhkeys.o xcrypt.o
UPDATEOBJS = update.o

DOMAINNAMEOBJS = domainname.o
K_OBJS = setkey.o detach.o key_generic.o keyserv_cache.o
CHANGE_OBJS  = update.o chkey_common.o
OUTSIDE_UTIL_OBJS = selfcheck.o

OBJS	= $(KEYSERVOBJS) $(KEYLOGOUTOBJS) $(KEYLOGINOBJS) $(CHKEYOBJS) \
	$(NEWKEYOBJS) $(UPDATEOBJS) $(DOMAINNAMEOBJS) \
	$(K_OBJS)  $(CHANGE_OBJS)
SRCS	= $(OBJS:.o=.c)

SED=	sed

include ../Makefile.cmd

IBINPROG=		$(BINPROG:%=$(ROOTBIN)/%)
ISBINPROG=		$(SBINPROG:%=$(ROOTUSRSBIN)/%)

ROOTMANIFESTDIR=	$(ROOTSVCNETWORKRPC)

$(ROOTBIN)/chkey :=				FILEMODE=	4555

$(ROOTKEYSERVDFLT) :=				FILEMODE=	444

CPPFLAGS += -I. -I$(SRC)/lib/libc/inc -I$(SRC)/lib/libsldap/common

CERRWARN +=	-Wno-implicit-function-declaration
CERRWARN +=	-Wno-parentheses
CERRWARN +=	-Wno-uninitialized
CERRWARN +=	-Wno-unused-variable
CERRWARN +=	-Wno-address
CERRWARN +=	-Wno-unused-function

# not linted
SMATCH=off

chkey :=	LDLIBS += -lsldap -lmp -lmd
keyserv :=	LDLIBS += -lmp -lmd
newkey  :=	LDLIBS += -lsldap -lmp -lmd

.KEEP_STATE:

all: $(PROG)

keyserv: $(K_OBJS)  $(KEYSERVOBJS)
	$(LINK.c) $(K_OBJS)  $(KEYSERVOBJS) -o $@ $(LDLIBS)
	$(POST_PROCESS)

keylogout: $(KEYLOGOUTOBJS)
	$(LINK.c) $(KEYLOGOUTOBJS) -o $@ $(LDLIBS)
	$(POST_PROCESS)

keylogin: $(KEYLOGINOBJS)
	$(LINK.c) $(KEYLOGINOBJS) -o $@ $(LDLIBS)
	$(POST_PROCESS)

chkey: $(CHKEYOBJS) $(CHANGE_OBJS)
	$(LINK.c) $(CHANGE_OBJS) $(CHKEYOBJS) -o $@ $(LDLIBS)
	$(POST_PROCESS)

newkey:$(CHANGE_OBJS)  $(NEWKEYOBJS) $(OUTSIDE_UTIL_OBJS)
	$(LINK.c) $(CHANGE_OBJS)  $(NEWKEYOBJS) $(OUTSIDE_UTIL_OBJS) \
	-o $@ $(LDLIBS)
	$(POST_PROCESS)

update: $(UPDATEOBJS)
	$(LINK.c)  $(UPDATEOBJS) -o $@ $(LDLIBS)
	$(POST_PROCESS)

domainname: $(DOMAINNAMEOBJS)
	$(LINK.c) $(DOMAINNAMEOBJS) -o $@ $(LDLIBS)
	$(POST_PROCESS)

selfcheck.o: ../fs.d/nfs/lib/selfcheck.c
	$(COMPILE.c) ../fs.d/nfs/lib/selfcheck.c

install: all $(DIRS) $(IBINPROG) $(ISBINPROG) $(ROOTETCDEFAULTFILES) \
	$(ROOTMANIFEST)

clean:
	$(RM) $(OBJS) $(OUTSIDE_UTIL_OBJS)


check:	$(CHKMANIFEST)

key_prot.h : $(ROOT)/usr/include/rpcsvc/key_prot.x
	$(RPCGEN) -h $(ROOT)/usr/include/rpcsvc/key_prot.x > key_prot.h

include ../Makefile.targ
