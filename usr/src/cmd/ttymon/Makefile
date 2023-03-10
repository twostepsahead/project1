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

PROG= stty ttymon ttyadm sttydefs

# the 'stty' objects have to be made in a separate directory
# since only they are built with the -DEUC flag (see below).
STTYOBJ=	sttyobjs/stty.o sttyobjs/sttytable.o sttyobjs/sttyparse.o

TTYMONOBJ=	ttymon.o tmglobal.o tmhandler.o tmpmtab.o tmttydefs.o \
		tmparse.o tmsig.o tmsac.o tmchild.o tmautobaud.o tmterm.o \
		tmutmp.o tmpeek.o tmlog.o tmlock.o tmutil.o tmexpress.o \
		sttytable.o sttyparse.o ulockf.o

TTYADMOBJ=	ttyadm.o tmutil.o admutil.o 

STTYDEFSOBJ=	sttydefs.o admutil.o tmttydefs.o tmparse.o sttytable.o \
		sttyparse.o 

OBJS= $(STTYOBJ) $(TTYMONOBJ) $(TTYADMOBJ) $(STTYDEFSOBJ)
STTYSRC= stty.c sttytable.c sttyparse.c
TTYMONSRC= $(TTYMONOBJ:%.o=%.c)
TTYADMSRC= $(TTYADMOBJ:%.o=%.c)
STTYDEFSSRC= $(STTYDEFSOBJ:%.o=%.c)
SRCS= $(STTYSRC) $(TTYMONSRC) $(TTYADMSRC) $(STTYDEFSSRC)

.PARALLEL:	$(OBJS) $(PROG)

include ../Makefile.cmd

# If machine name and /etc/issue file need to be printed 
# before the service prompt is printed, then:  CPPFLAGS += -DSYS_NAME
# If debug is needed, then:  CPPFLAGS += -DDEBUG
# fix for 1111333 - turn on SYS_NAME so /etc/issue will be read if it exists
CPPFLAGS += -DSYS_NAME
CPPFLAGS += -DXPG4
ttymon := LDLIBS += -lsec -ldevinfo

# Only stty can be built with -DEUC.  ttymon will dump core unless further
# changes are made to it.
$(STTYOBJ) := CPPFLAGS += -DEUC

CERRWARN +=	-Wno-parentheses
CERRWARN +=	-Wno-unused-variable
CERRWARN +=	-Wno-uninitialized
CERRWARN +=	-Wno-extra

# not linted
SMATCH=off

#
# Message catalog
#
POFILES= $(STTYOBJ:sttyobjs/%.o=%.po)
POFILE= ttymon.po


LIBSAFD= $(ROOTLIB64)/saf
DIRS= $(LIBSAFD)
LIBSAF= ttymon
USRSBINF= sttydefs ttyadm
BINF= stty
ROOTLIBF= $(LIBSAF:%=$(LIBSAFD)/%)
ROOTUSRSBINF= $(USRSBINF:%=$(ROOTUSRSBIN)/%)
ROOTBINF= $(BINF:%=$(ROOTBIN)/%)
$(ROOTLIBF) :=			FILEMODE =	0555
$(ROOTUSRSBINF) :=		FILEMODE =	0755

$(LIBSAFD)/% : %
	$(INS.file)

.KEEP_STATE:

all: $(PROG)

stty:	sttyobjs $(STTYOBJ)
	$(LINK.c) $(STTYOBJ) -o $@ $(LDLIBS)
	$(POST_PROCESS)

sttyobjs/%.o:	%.c
	$(COMPILE.c) -o $@ $<

sttyobjs:
	-@mkdir -p $@

ttymon:	$(TTYMONOBJ)
	$(LINK.c) $(TTYMONOBJ) -o $@ $(LDLIBS) -lpam
	$(POST_PROCESS)

ttyadm:	$(TTYADMOBJ)
	$(LINK.c) $(TTYADMOBJ) -o $@ $(LDLIBS)
	$(POST_PROCESS)

sttydefs: $(STTYDEFSOBJ)
	$(LINK.c) $(STTYDEFSOBJ) -o $@ $(LDLIBS)
	$(POST_PROCESS)

install: all $(DIRS) $(ROOTLIBF) $(ROOTUSRSBINF) $(ROOTBINF)

$(POFILE):      $(POFILES)
	$(RM) $@
	cat $(POFILES)  > $@

$(DIRS):
	$(INS.dir)

clean:
	$(RM) $(OBJS)

include ../Makefile.targ
