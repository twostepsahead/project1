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

PROG=	ldap_cachemgr

MANIFEST=	client.xml
SVCMETHOD=	ldap-client

include ../Makefile.cmd

ROOTLDAPLIB=	$(ROOT)/usr/lib/ldap
ROOTLDAPPROG=	$(PROG:%=$(ROOTLDAPLIB)/%)

ROOTMANIFESTDIR=	$(ROOTSVCNETWORKLDAP)

OBJS=	cachemgr.o cachemgr_getldap.o cachemgr_parse.o cachemgr_change.o

SRCS=	${OBJS:%.o=%.c} 

CPPFLAGS += 	-DSUN_THREADS \
		-I$(SRC)/lib/libsldap/common \
		-I$(SRC)/lib/libldap5/include/ldap \
		-I$(SRC)/lib/libc/port/gen

CERRWARN +=	-Wno-parentheses
CERRWARN +=	-Wno-switch
CERRWARN +=	-Wno-uninitialized

# not linted
SMATCH=off

# Message files
POFILE=		ldap_cachemgr.po
POFILES=	${OBJS:%.o=%.po}


# TCOV_FLAG=	-ql
# GPROF_FLAG=	-pg
# DEBUG_FLAG=	-g

LDLIBS += 	-lumem -lsldap

# install macros and rule
#

.KEEP_STATE:

clean := TARGET= clean
clobber := TARGET= clobber

$(ROOTSVCMETHOD) := FILEMODE = 0555

all: $(PROG)

${PROG}: ${OBJS}
	${LINK.c} ${OPT} -o $@ ${OBJS} ${LDLIBS}
	${POST_PROCESS}

$(ROOTLDAPLIB):
	$(INS.dir)

$(ROOTLDAPLIB)/%:	%
	$(INS.file)

cstyle:
	${CSTYLE} ${SRCS}

install: all $(ROOTLDAPLIB) $(ROOTLDAPPROG) $(ROOTMANIFEST) $(ROOTSVCMETHOD)

check:	$(CHKMANIFEST)

clean:
	$(RM) $(OBJS)

$(POFILE): $(POFILES)
	$(RM) $@
	cat $(POFILES) > $@

include $(SRC)/cmd/Makefile.targ
