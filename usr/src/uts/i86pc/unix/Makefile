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
# Copyright 2008 Sun Microsystems, Inc.  All rights reserved.
# Use is subject to license terms.
#
# Copyright (c) 2018, Joyent, Inc.

#
#	Path to the base of the uts directory tree (usually /usr/src/uts).
#
UTSBASE	= ../..

#
#	Define the module and object file sets.
#
UNIX		= unix
DBOOT		= dboot

OBJECTS		= $(SPECIAL_OBJS:%=$(OBJS_DIR)/%) \
		  $(CORE_OBJS:%=$(OBJS_DIR)/%) \
		  $(KRTLD_OBJS:%=$(OBJS_DIR)/%) \
		  $(MACH_NOT_YET_KMODS:%=$(OBJS_DIR)/%)

ROOTMODULE	= $(ROOT_PSM_KERN_DIR)/$(UNIX)

UNIX_BIN	= $(OBJS_DIR)/$(UNIX)

LIBS		= $(GENLIB)

GENUNIX		= genunix
GENUNIX_DIR	= ../../intel/$(GENUNIX)

LIBOPTS		= -L $(GENUNIX_DIR)/$(OBJS_DIR) -l $(GENUNIX)

COMMP_CTF_SRC	= $(OBJS_DIR)/comm_page_ctf.c

CTFEXTRAOBJS	= $(OBJS_DIR)/vers.o $(OBJS_DIR)/comm_page_ctf.o

DBOOT_OBJS_DIR	= dboot/$(OBJS_DIR)
DBOOT_OBJECTS	= $(DBOOT_OBJS:%=$(DBOOT_OBJS_DIR)/%)
DBOOT_BIN	= $(DBOOT_OBJS_DIR)/$(DBOOT)
DBOOT_O		= $(OBJS_DIR)/$(DBOOT).o
DBOOT_S		= $(DBOOT_O:%.o=%.s)

#
#	Include common rules.
#
include $(UTSBASE)/i86pc/Makefile.i86pc

#
#	Define targets
#
ALL_TARGET	= $(UNIX_BIN)
INSTALL_TARGET	= $(UNIX_BIN) $(ROOTMODULE)

#
#	This is UNIX_DIR. Use a short path.
#
UNIX_DIR	= .

#
#	Overrides
#
CLEANFILES	+=		\
	$(UNIX_O) $(MODSTUBS_O)	\
	$(OBJS_DIR)/vers.c \
	$(OBJS_DIR)/dtracestubs.s \
	$(DTRACESTUBS_O) $(DTRACESTUBS) \
	$(CTFEXTRAOBJS) \
	$(COMMP_CTF_SRC)

CLEANFILES	+=		\
	$(DBOOT_O) $(DBOOT_S)	\
	$(DBOOT_OBJECTS)	\
	$(OBJS_DIR)/bios_call_src.o \
	$(OBJS_DIR)/bios_call_src \
	$(OBJS_DIR)/bios_call.s \
	$(DBOOT_BIN)

CLEANFILES	+=		\
	$(DBOOT_OBJS_DIR)/$(VGATEXT_FONT).c \
	$(OBJS_DIR)/$(VGATEXT_FONT).c

CLEANFILES	+=		\
	$(OBJS_DIR)/fb_swtch_src.o \
	$(OBJS_DIR)/fb_swtch_src \
	$(OBJS_DIR)/fb_swtch.s

CLEANFILES	+= \
	$(ZLIB_OBJS:%.o=$(OBJS_DIR)/%.o) \
	$(ZLIB_OBJS:%.o=$(OBJS_DIR)/%.ln)

CLOBBERFILES	= $(CLEANFILES) $(UNIX_BIN)

# instr_size needs a special header
$(OBJS_DIR)/instr_size.o :=	EXTRA_OPTIONS	= -I$(SRC)/common/dis/i386
$(OBJS_DIR)/instr_size.ln :=	EXTRA_OPTIONS	= -I$(SRC)/common/dis/i386

CFLAGS += -DDIS_MEM

$(OBJS_DIR)/decompress.o :=	EXTRA_OPTIONS	= -I$(SRCTOP)/kernel/zmod

#
# For now, disable these compiler warnings; maintainers should endeavor to
# investigate and remove these for maximum coverage.  Please do not carry
# these forward to new Makefiles.
#

CERRWARN	+= -Wno-parentheses
CERRWARN	+= -Wno-uninitialized
CERRWARN	+= -Wno-char-subscripts
CERRWARN	+= -Wno-unused-variable
CERRWARN	+= -Wno-unused-function
CERRWARN	+= -Wno-unused-label
CERRWARN	+= -Wno-type-limits
CERRWARN	+= -Wno-clobbered
CERRWARN	+= -Wno-empty-body
CERRWARN	+= -Wno-unused-value

# false positives
SMOFF += index_overflow

# needs work
SMOFF += all_func_returns,deref_check,signed

$(OBJS_DIR)/fmsmb.o := SMOFF += indenting
$(OBJS_DIR)/zutil.o := SMOFF += indenting
$(OBJS_DIR)/bootrd_cpio.o := SMOFF += allocating_enough_data

#
#	Default build targets.
#
.KEEP_STATE:

def: $(DEF_DEPS)

all: $(ALL_DEPS)

clean: $(CLEAN_DEPS)

clobber: $(CLOBBER_DEPS)

install: $(INSTALL_DEPS)

$(UNIX_BIN):	$(UNIX_O) $(MODSTUBS_O) $(MAPFILE) \
		$(GENLIB) $(DTRACESTUBS) $(DBOOT_O)
	$(LD) -dy -b -o $@ -e dboot_image -znointerp -M $(MAPFILE) \
	    $(UNIX_O) $(DBOOT_O) $(MODSTUBS_O) $(LIBOPTS) \
	    $(DTRACESTUBS)
	$(MBH_PATCH) $(UNIX_BIN)
	$(CTFMERGE_MODULE)
	$(POST_PROCESS)

$(UNIX_O):	$(OBJECTS) $(CTFEXTRAOBJS)
	$(LD) -r -o $@ $(OBJECTS) $(OBJS_DIR)/vers.o

$(DBOOT_BIN):	$(DBOOT_OBJS_DIR) $(DBOOT_OBJECTS) \
		$(SRCTOP)/arch/x86/kernel/dboot/mapfile-dboot
	$(LD) -dn -e _start -M \
		$(SRCTOP)/arch/x86/kernel/dboot/mapfile-dboot \
		-o $(DBOOT_BIN) $(DBOOT_OBJECTS)

$(DBOOT_O):	$(DBOOT_BIN)
	@echo "	.data"					> $(DBOOT_S)
	@echo "	.globl	dboot_image"			>> $(DBOOT_S)
	@echo "dboot_image:"				>> $(DBOOT_S)
	$(ELFEXTRACT) $(DBOOT_BIN)			>> $(DBOOT_S)
	$(COMPILE64.s) -o $(DBOOT_O) $(DBOOT_S)

$(DBOOT_OBJS_DIR):
	-@mkdir -p $@ 2> /dev/null

$(COMMP_CTF_SRC):	$(UTSBASE)/i86pc/ml/comm_page.s
	$(COMPILE.cpp) -D_GENCTF -o $@ $(UTSBASE)/i86pc/ml/comm_page.s

$(OBJS_DIR)/comm_page_ctf.o:	$(COMMP_CTF_SRC)
	$(COMPILE.c) -o $@ $<
	$(CTFCONVERT_O)

#
#	Special rules for generating assym.h for inclusion in assembly files.
#
$(DSF_DIR)/$(OBJS_DIR)/assym.h $(DSF_DIR)/$(OBJS_DIR)/kdi_assym.h:	FRC
	@cd $(DSF_DIR); $(MAKE) all.targ

#
#	Include common targets.
#
include $(UTSBASE)/i86pc/Makefile.targ
