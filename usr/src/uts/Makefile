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
# Copyright (c) 1989, 2010, Oracle and/or its affiliates. All rights reserved.
# Copyright 2014 Garrett D'Amore <garrett@damore.org>
# Copyright 2018 Nexenta Systems, Inc.
#

include ../Makefile.master

#
# List of architectures to build as part of the standard build.
#
# Some of these architectures are built in parallel (see i386_PARALLEL).
# This requires building some parts first before parallel build can start.
# Platform make files know what should be built as a prerequisite for the
# parallel build to work. The i386_PREREQ and sparc_PREREQ variables tell
# which platform directory to enter to start making prerequisite dependencies.
#
i386_ARCHITECTURES = i86pc intel

#
# For i386 all architectures can be compiled in parallel.
#
# intel/Makefile knows how to build prerequisites needed for parallel build.
#
i386_PREREQ = intel
i386_PARALLEL = $(i386_ARCHITECTURES)

#
# Platforms defined in $(MACH)_PARALLEL are built in parallel. DUMMY is placed
# at the end in case $(MACH)_PARALLEL is empty to prevent everything going in
# parallel.
#
.PARALLEL: $($(MACH)_PARALLEL) DUMMY

#
# For build prerequisites we use a special target which is constructed by adding
# '.prereq' suffix to the $(MACH)_PREREQ.
#
PREREQ_TARGET = $($(MACH)_PREREQ:%=%.prereq)


def		:=	TARGET= def
all		:=	TARGET= all
install		:=	TARGET= install
install_h	:=	TARGET= install_h
clean		:=	TARGET= clean
clobber		:=	TARGET= clobber
clobber_h	:=	TARGET= clobber
check		:=	TARGET= check
modlist		:=	TARGET= modlist
modlist		:=	NO_STATE= -K $$MODSTATE$$$$

.KEEP_STATE:

def all: all_h $(PMTMO_FILE) $($(MACH)_ARCHITECTURES)

install: all_h install_dirs $(PMTMO_FILE) $($(MACH)_ARCHITECTURES)

install_dirs:
	@cd ..; pwd; $(MAKE) rootdirs
	@pwd

#
# Rule to build prerequisites. The left part of the pattern will match
# PREREQ_TARGET.
#
# The location of the Makefile is determined by strippinng '.prereq' suffix from
# the target name. We add '.prereq' suffix to the target passed to the child
# Makefile so that it can distinguish prerequisite build from the regular one.
#
#
%.prereq:
	@cd $(@:%.prereq=%); pwd; $(MAKE) $(NO_STATE) $(TARGET).prereq

#
# Rule to build architecture files. Build all required prerequisites and then
# build the rest (potentially in parallel).
#
$($(MACH)_ARCHITECTURES): $(PREREQ_TARGET) FRC
	@cd $@; pwd; $(MAKE) $(NO_STATE) $(TARGET)


#
# The following is the list of directories which contain Makefiles with
# targets to install header file. The machine independent headers are
# installed by invoking the Makefile in the directory containing the
# header files. Machine and architecture dependent headers are installed
# by invoking the main makefile for that architecture/machine which,
# in turn, is responsible for invoking the Makefiles which install headers.
# It is done this way so as not to assume that all of the header files in
# the architecture/machine dependent subdirectories are in completely
# isomorphic locations.
#
COMMON_HDRDIRS= common/des \
		common/gssapi \
		common/idmap \
		common/klm \
		common/io/bpf/net \
		common/io/fibre-channel/fca/qlc \
		common/ipp \
		common/netinet \
		common/nfs \
		common/rpc \
		common/rpcsvc \
		common/sharefs \
		common/smb \
		common/smbsrv


#
# Subset of COMMON_HDRDIRS in which at least one header is generated
# at runtime (e.g., rpcgen), and in which "make clean" should run.
# Other directories should be included here, but do not yet have the
# necessary Makefile support (make clean).
#
DYNHDRDIRS =	common/gssapi \
		common/idmap \
		common/io/fibre-channel/fca/qlc \
		common/klm \
		common/rpc \
		common/rpcsvc

i386_HDRDIRS=

HDRDIRS= $(COMMON_HDRDIRS) $($(MACH)_HDRDIRS)
install_h check: $(HDRDIRS) $($(MACH)_ARCHITECTURES)

$(HDRDIRS): FRC
	@cd $@; pwd; $(MAKE) $(TARGET)

# ensures that headers made by rpcgen and others are available in uts source
# for kernel builds to reference without building install_h
#
all_h: FRC
	@cd common/rpc; pwd; $(MAKE) $@
	@cd common/rpcsvc; pwd; $(MAKE) $@
	@cd common/gssapi; pwd; $(MAKE) $@
	@cd common/idmap; pwd; $(MAKE) $@
	@cd common/klm; pwd; $(MAKE) $@

clean clobber: $($(MACH)_ARCHITECTURES) $(DYNHDRDIRS)

# testing convenience
clobber_h: $(DYNHDRDIRS)

modlist: $($(MACH)_ARCHITECTURES)

FRC:
