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
# Copyright 2015 Gary Mills
# Copyright 2009 Sun Microsystems, Inc.  All rights reserved.
# Use is subject to license terms.
# Copyright (c) 2018, Joyent, Inc.
#

BINPROG = ypmatch ypwhich ypcat
SBINPROG = makedbm ypalias ypinit yppoll ypset
NETYPPROG = ypbind ypxfr yppush udpublickey mkalias \
		multi multi.awk stdethers stdhosts \
		ypxfr_1perday ypxfr_1perhour ypxfr_2perday \
		ypstart ypstop
NIS2LDAPPROG = ypxfrd ypserv ypxfr
SVCMETHOD = yp
PROG=	$(BINPROG) $(SBINPROG) $(NETYPPROG) $(NIS2LDAPPROG) $(SVCMETHOD)

MANIFEST = server.xml client.xml xfr.xml

include ../Makefile.cmd

ROOTMANIFESTDIR = $(ROOTSVCNETWORKNIS)

# installed directories
NETSVC = $(ROOTLIB64)/netsvc
NETYP = $(NETSVC)/yp

ROOTVAR_YP = $(ROOT)/var/yp
ROOTBINDING = $(ROOTVAR_YP)/binding
ROOTDIRS =	$(ROOT) $(ROOTUSR) $(ROOTLIB64) $(ROOTETC) $(ROOTVAR) \
		$(NETSVC) $(NETYP) $(ROOTVAR_YP) $(ROOTBINDING) \
		$(ROOTUSRSBIN) $(ROOTBIN)

$(NIS2LDAPPROG) := LDLIBS += -lc -lnisdb
makedbm :=	MAPFILES = $(MAPFILE.INT)
makedbm :=	LDFLAGS += $(MAPFILES:%=-Wl,-M%)

$(ROOTVAR_YP)/aliases :=	FILEMODE=555

YPFILES = Makefile aliases nicknames updaters
ETCFILES =  publickey
ROOTYPFILES= $(YPFILES:%=$(ROOTVAR_YP)/%)
ROOTETCFILE= $(ETCFILES:%=$(ROOTETC)/%)

TXTS=	$(ETCFILES:%=net_files/%) $(YPFILES:%=net_files/%)
SUBDIRS = yppasswd mknetid revnetgroup ypupdated yp2lscripts

YPBINDOBJ= yp_b_svc.o yp_b_subr.o rpc_bootstrap.o getlist.o
#
# Some sort of obsolete idea of common files which are only now used by a 
# few minor utilities
#
COMMONOBJ= getlist.o yp_getalias.o 

#
# Objects shared between all the major components
#
SHARED_DIR= ./shared
SHAREDOBJ= $(SHARED_DIR)/utils.o $(SHARED_DIR)/lockmap.o $(SHARED_DIR)/ancil.o
SHARESRC = $(SHAREOBJ:.o=.c)

YPXFROBJ = ypxfr.o ypxfrd_client.o ypxfrd_xdr.o
YPMATOBJ = ypmatch.o nick.o
YPWHIOBJ = ypwhich.o nick.o
YPCATOBJ = ypcat.o nick.o
UDPUBLICKEYOBJ = udpublickey.o
MKALIASOBJ = mkalias.o
STDETHERSOBJ = stdethers.o
STDHOSTSOBJ = stdhosts.o
#COMMONCLNTOBJ =	clnt_create_rsvd.o
YPSERVOBJ = ypserv.o ypserv_map.o \
		ypserv_proc.o ypserv_ancil.o \
		ypserv_resolv.o ypserv_resolv_common.o \
		ypv1_xdr.o ypserv_net_secure.o
YPPUSHOBJ = yppush.o
YPXFRDOBJ = ypxfrd_svc.o ypxfrd_xdr.o ypxfrd_server.o \
		ypserv_net_secure.o

NETYPOBJ = $(YPXFROBJ) $(YPXFRDOBJ) $(YPSERVOBJ) $(YPPUSHOBJ) \
		$(YPBINDOBJ) $(UDPUBLICKEYOBJ)
OBJS = $(NETYPOBJ) $(MKALIASOBJ) $(COMMONOBJ) $(SHAREDOBJ) \
	$(YPPUSHOBJ) $(YPMATOBJ) $(YPWHIOBJ) $(YPCATOBJ) \
	$(STDETHERSOBJ) $(STDHOSTSOBJ) makedbm.o ypalias.o pong.o

BINSRC = $(BINPROG:=.c)

SBINSRC = $(SBINPROG:=.c)

NETYPSRC = $(NETYPOBJ:.o=.c)

YPBINDSRC = $(YPBINSOBJ:.o=.c)

YPXFRSRC =	$(YPXFROBJ:.o=.c)

YPSERVSRC =	$(YPSERVOBJ:.o=.c)

YPPUSHSRC =	$(YPPUSHOBJ:.o=.c)

YPXFRDSRC =	$(YPXFRDOBJ:.o=.c)

YPMASTERSRC =	$(YPMASTEROBJ:.o=.c)

YPALLSRC =	$(YPALLOBJ:.o=.c)

COMMONSRC = 	$(COMMONOBJ:.o=.c)

#COMMONCLNTSRC =	$(COMMONCLNTOBJ:.o=.c)

SRCS = $(BINSRC) $(SBINSRC) $(NETYPSRC) \
	$(COMMONSRC) $(SHAREDSRC)

CLEANFILES =	ypxfrd.h ypxfrd_xdr.c 

IBINPROG=		$(BINPROG:%=$(ROOTBIN)/%)
ISBINPROG=		$(SBINPROG:%=$(ROOTUSRSBIN)/%)
INETYPPROG=		$(NETYPPROG:%=$(NETYP)/%)
INIS2LDAPPROG = 	$(NIS2LDAPPROG:%=$(NETYP)/%)

NIS2LDAPINC =		-I$(SRC)/lib/libnisdb/yptol

CERRWARN +=	-Wno-implicit-function-declaration
CERRWARN +=	-Wno-parentheses
CERRWARN +=	-Wno-unused-variable
CERRWARN +=	-Wno-uninitialized
CERRWARN +=	-Wno-switch
CERRWARN +=	-Wno-extra
CERRWARN +=	-Wno-unused-label
CERRWARN +=	-Wno-address
CERRWARN +=	-Wno-unused-function

# too many unknown types
SMATCH =	off

#conditional assignments
ypalias.o :=	CPPFLAGS= -DMAIN $(CPPFLAGS.master)
makedbm.o :=	CPPFLAGS= $(CPPFLAGS.master)
yp_b_svc.o :=	CPPFLAGS += -DINIT_DEFAULT
ypxfrd_svc.o := CPPFLAGS += -Dmain=_main
ypxfr.o := CPPFLAGS += $(NIS2LDAPINC)
yppush.o := CPPFLAGS += $(NIS2LDAPINC)
$(NIS2LDAPPROG) := CPPFLAGS += $(NIS2LDAPINC)
$(SHAREDOBJ) := CPPFLAGS += -I$(SRC)/lib/libnisdb/yptol
$(ROOTVAR_YP)/Makefile := FILEMODE= 555
$(ROOTVAR_YP)/nicknames := FILEMODE= 644
$(ROOTVAR_YP)/updaters := FILEMODE= 500
$(ROOTETC)/publickey := FILEMODE= 644
# non-default file attributes to avoid conflict with rpcsrc
NETYP =		$(NETSVC)/yp


all:=		TARGET= all
install:=	TARGET= install
clean:=		TARGET= clean
clobber:=	TARGET= clobber
cstyle :=	TARGET= cstyle

all: $(SHAREDOBJ) $(SUBDIRS) $(PROG) $(TXTS) 

# install rules
$(ROOTVAR_YP)/% \
$(ROOTETC)/%: net_files/%
	$(INS.file)

$(ROOTVAR_YP)/% : net_files/%
	$(INS.file)

$(NETYP)/%: %
	$(INS.file)

.KEEP_STATE:

$(SHAREDOBJ): $(SHAREDSRC)
	$(COMPILE.c) $(SHAREDSRC) -o $@ $<

ypbind: $(YPBINDOBJ)
	$(LINK.c) $(YPBINDOBJ) -o $@ $(LDLIBS)
	$(POST_PROCESS)

makedbm ypalias: $$@.o $(COMMONOBJ) $$(MAPFILES)
	$(LINK.c) -o $@ $@.o $(COMMONOBJ) $(LDLIBS)
	$(POST_PROCESS)

ypmatch: $(YPMATOBJ)
	$(LINK.c) -o $@ $(YPMATOBJ) $(LDLIBS)
	$(POST_PROCESS)

ypwhich: $(YPWHIOBJ)
	$(LINK.c) -o $@ $(YPWHIOBJ) $(LDLIBS)
	$(POST_PROCESS)

ypcat: $(YPCATOBJ)
	$(LINK.c) -o $@ $(YPCATOBJ) $(LDLIBS)
	$(POST_PROCESS)

ypxfrd_xdr.c: ypxfrd.x
	$(RM) ypxfrd_xdr.c; $(RPCGEN) -C -c ypxfrd.x -o ypxfrd_xdr.c

ypxfrd.h: ypxfrd.x
	$(RM) ypxfrd.h; $(RPCGEN) -C -h ypxfrd.x -o ypxfrd.h

ypxfrd_xdr.o ypxfrd_client.o: ypxfrd.h

#clnt_create_rsvd.o ypxfr.o: clnt_create_rsvd.h

ypxfrd: $(YPXFRDOBJ) $(SHAREDOBJ)
	$(LINK.cc) $(YPXFRDOBJ) $(SHAREDOBJ) \
	-o $@ $(LDLIBS) -lc
	$(POST_PROCESS)

ypxfr: $(YPXFROBJ) $(COMMONOBJ) $(YPMASTEROBJ) $(SHAREDOBJ)
	$(LINK.cc) -o $@ $(YPXFROBJ) $(YPMASTEROBJ) $(COMMONOBJ) \
		$(SHAREDOBJ) $(LDLIBS)
	$(POST_PROCESS)

ypserv: $(YPSERVOBJ) $(COMMONOBJ) $(SHAREDOBJ)
	$(LINK.cc) $(YPSERVOBJ) $(COMMONOBJ) \
		$(SHAREDOBJ) -o $@ $(LDLIBS)
	$(POST_PROCESS)

yppush: $(YPPUSHOBJ) $(COMMONOBJ)
	$(LINK.c) $(YPPUSHOBJ) $(COMMONOBJ) -o $@ $(LDLIBS)
	$(POST_PROCESS)

udpublickey: $(UDPUBLICKEYOBJ)
	$(LINK.c) $(UDPUBLICKEYOBJ) -o $@ $(LDLIBS)
	$(POST_PROCESS)

mkalias: $(MKALIASOBJ)
	$(LINK.c) $(MKALIASOBJ) -o $@ $(LDLIBS)
	$(POST_PROCESS)

stdethers: $(STDETHERSOBJ)
	$(LINK.c) $(STDETHERSOBJ) -o $@ $(LDLIBS)
	$(POST_PROCESS)

stdhosts: $(STDHOSTSOBJ)
	$(LINK.c) $(STDHOSTSOBJ) -o $@ $(LDLIBS)
	$(POST_PROCESS)

install: all $(PROG) $(ROOTDIRS) $(ROOTETCFILE) $(ROOTYPFILES) $(IBINPROG) \
	$(ISBINPROG) $(INETYPPROG) $(INIS2LDAPPROG) $(SUBDIRS) $(ROOTMANIFEST) \
	$(ROOTSVCMETHOD)

$(ROOTDIRS):
	$(INS.dir)

$(SUBDIRS): FRC
	@cd $@; pwd; $(MAKE) $(TARGET)

check: $(CHKMANIFEST)

cstyle:	$(SUBDIRS)
	${CSTYLE} `echo $(SRCS) | sed 's/ypinit.c//'`

clean:	$(SUBDIRS)
	$(RM) $(OBJS) $(CLEANFILES)

clobber: clean $(SUBDIRS)

FRC:

include ../Makefile.targ
