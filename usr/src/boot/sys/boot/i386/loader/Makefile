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

#
# Copyright 2015 Toomas Soome <tsoome@me.com>
# Copyright 2016 RackTop Systems.
#

include $(SRC)/Makefile.master
include $(SRC)/boot/Makefile.version
include $(SRC)/boot/sys/boot/Makefile.inc

CFLAGS=		-Os -fno-reorder-functions
CPPFLAGS=	-D_STANDALONE -nostdinc -I../../../../include -I../../..
CPPFLAGS +=	-I$(SRCTOP)/arch/x86/include/sys/acpi
CPPFLAGS +=	-I../../../../lib/libz
LOADER=		loader
NEWVERSWHAT=	"ZFS enabled bootstrap loader" x86

# Set by loader Makefile
CPPFLAGS +=	-I../../zfs
CPPFLAGS +=	-I../libi386
LIBZFSBOOT=	../../zfs/$(MACH)/libzfsboot.a
LIBI386=        ../libi386/libi386.a

LD=		$(GNU_ROOT)/bin/gld
CC=		$(GNUC_ROOT)/bin/gcc
OBJCOPY=	$(GNU_ROOT)/bin/gobjcopy
GSTRIP=		$(GNU_ROOT)/bin/gstrip
ROOT_BOOT=	$(ROOT)/boot
ROOT_BOOT_DEFAULTS=	$(ROOT)/boot/defaults
ROOT_BOOT_FORTH=	$(ROOT)/boot/forth
ROOT_BOOT_CONF=		$(ROOT)/boot/conf.d
ROOTBOOTFILES=$(FILES:%=$(ROOT_BOOT)/%)
ROOTBOOTFORTH=$(FORTH:%=$(ROOT_BOOT_FORTH)/%)
ROOTBOOTDEFAULTS=$(DEFFILES:%=$(ROOT_BOOT_DEFAULTS)/%)
FILEMODE=0444

all: ${LOADER} loader.help

install: all $(ROOTBOOTLOADER)

PROG=		${LOADER}.sym

# architecture-specific loader code
SRCS=		main.c conf.c vers.c chain.c

CPPFLAGS +=	-DLOADER_GZIP_SUPPORT

# Enable BootForth
CPPFLAGS +=	-DBOOT_FORTH -I$(SRC)/common/ficl -I../../libficl
LIBFICL=	../../libficl/$(MACH)/libficl.a

# Always add MI sources
SRCS +=	boot.c commands.c console.c devopen.c interp.c
SRCS +=	interp_backslash.c interp_parse.c ls.c misc.c
SRCS +=	module.c panic.c linenoise.c multiboot2.c
SRCS +=	zfs_cmd.c
SRCS +=	font.c $(FONT).c list.c tem.c

SRCS +=	load_elf32.c load_elf32_obj.c reloc_elf32.c
SRCS +=	load_elf64.c load_elf64_obj.c reloc_elf64.c

SRCS +=	dev_net.c

SRCS +=	disk.c part.c
CPPFLAGS += -DLOADER_DISK_SUPPORT
CPPFLAGS += -DLOADER_GPT_SUPPORT
CPPFLAGS += -DLOADER_MBR_SUPPORT

SRCS +=  bcache.c

SRCS +=	isapnp.c
SRCS +=	pnp.c

# Forth interpreter
SRCS +=	interp_forth.c

CPPFLAGS +=	-I../../common
CPPFLAGS +=	-I.

CLEANFILES=	vers.c ${LOADER} ${LOADER}.sym ${LOADER}.bin loader.help
CLEANFILES +=	$(FONT).c

CFLAGS +=	-Wall
LDFLAGS=	-static -Ttext 0x0 -m elf_i386

# i386 standalone support library
CPPFLAGS +=	-I.. -I../../../../lib/libstand
LIBSTAND=	../../libstand/$(MACH)/libstand.a
LIBCRYPTO=	../../libcrypto/$(MACH)/libcrypto.a

# BTX components
CPPFLAGS +=	-I../btx/lib

# Debug me!
#CFLAGS+=	-g
#LDFLAGS+=	-g

include ../Makefile.inc

# For multiboot2.h, must be last, to avoid conflicts
CPPFLAGS +=	-I$(SRCTOP)/include

vers.c:	../../common/newvers.sh $(SRC)/boot/Makefile.version
	$(SH) ../../common/newvers.sh ${LOADER_VERSION} ${NEWVERSWHAT}

${LOADER}: ${LOADER}.bin ${BTXLDR} ${BTXKERN}
	$(BTXLD) -f aout -e ${LOADER_ADDRESS} -o $@ -l ${BTXLDR} \
		-b ${BTXKERN} ${LOADER}.bin

${LOADER}.bin: ${LOADER}.sym
	$(CP) $^ $@
	$(GSTRIP) -R .comment -R .note $@

loader.help: ../../common/help.common help.i386
	$(CAT) $^ | $(AWK) -f ../../common/merge_help.awk > $@

FILES=	${LOADER} loader.help

include	../../forth/Makefile.inc
FORTH +=	pcibios.4th

FILES +=	loader.rc
FORTH +=	menu.rc

# XXX crt0.o needs to be first for pxeboot(8) to work

DPADD=	${LIBFICL} ${LIBZFSBOOT} ${LIBI386} ${LIBCRYPTO} ${LIBSTAND}
LDADD=	${LIBFICL} ${LIBZFSBOOT} ${LIBI386} ${LIBCRYPTO} ${LIBSTAND}

CLEANFILES +=	machine x86

machine:
	$(RM) machine
	$(SYMLINK) ../../../i386/include machine

x86:
	$(RM) x86
	$(SYMLINK) ../../../x86/include x86

OBJS=		$(SRCS:%.c=%.o)

.PARALLEL:

${OBJS}: machine x86

${PROG}: ${OBJS} $(LDADD)
	$(LD) $(LDFLAGS) -o $@ $(BTXCRT) $(OBJS) $(LDADD)

clean: clobber
clobber:
	$(RM) $(CLEANFILES) $(OBJS)

install: all $(ROOT_BOOT_DEFAULTS) $(ROOT_BOOT_FORTH) \
	$(ROOTBOOTFILES) $(ROOTBOOTDEFAULTS) $(ROOT_BOOT_CONF) $(ROOTBOOTFORTH)

%.o:	../../common/%.c
	$(COMPILE.c) -o $@ $<

%.o:	../../common/linenoise/%.c
	$(COMPILE.c) -o $@ $<

%.o: $(SRC)/common/list/%.c
	$(COMPILE.c) -DNDEBUG $<

%.o: $(SRC)/common/font/%.c
	$(COMPILE.c) $<

$(FONT).c: $(FONT_DIR)/$(FONT_SRC)
	$(VTFONTCVT) -f compressed-source -o $@ $(FONT_DIR)/$(FONT_SRC)

$(ROOT_BOOT)/%: ../../forth/% $(ROOT_BOOT)
	$(INS.file)

$(ROOT_BOOT_DEFAULTS)/%: ../../forth/% $(ROOT_BOOT_DEFAULTS)
	$(INS.file)

$(ROOT_BOOT_FORTH)/%: ../../forth/% $(ROOT_BOOT_FORTH)
	$(INS.file)

$(ROOT_BOOT_DEFAULTS) $(ROOT_BOOT_CONF) $(ROOT_BOOT_FORTH):
	$(INS.dir)
