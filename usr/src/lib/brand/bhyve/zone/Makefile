#
# This file and its contents are supplied under the terms of the
# Common Development and Distribution License ("CDDL"), version 1.0.
# You may only use this file in accordance with the terms of version
# 1.0 of the CDDL.
#
# A full copy of the text of the CDDL should have accompanied this
# source.  A copy of the CDDL is also available via the Internet at
# http://www.illumos.org/license/CDDL.
#
# Copyright 2020 Joyent, Inc.
#

include $(SRC)/cmd/Makefile.cmd
include ../Makefile.bhyve
include $(SRC)/cmd/Makefile.cmd.64

PROGS =		attach detach statechange uninstall

PROG1 =		boot
PROG2 =		bhhwcompat
PROGS +=	$(PROG1) $(PROG2)
CLEANFILES +=	$(PROG1) $(PROG2)

$(PROG1) :=	LDLIBS += -lnvpair -lcustr
$(PROG2) :=	CPPFLAGS = -I$(COMPAT)/bhyve -I$(CONTRIB)/bhyve \
	$(CPPFLAGS.master) -I$(SRC)/uts/intel \
	-I$(COMPAT)/bhyve/amd64 -I$(CONTRIB)/bhyve/amd64

TEMPLATES =	SYSbhyve.xml
XMLDOCS =	config.xml platform.xml
CLOBBERFILES =	$(ROOTXMLDOCS) $(ROOTTEMPLATES) $(ROOTPROGS)

# This shouldn't be necessary, but for some reason the same thing in
# ../../Makefile.brand is not being picked up.
$(ROOTPROGS) :=	FILEMODE = 755

all: $(PROGS)

install: $(PROGS) $(ROOTPROGS) $(ROOTXMLDOCS) $(ROOTTEMPLATES)

clean:
	$(RM) $(CLEANFILES)

include $(SRC)/cmd/Makefile.targ
