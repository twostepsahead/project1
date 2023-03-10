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

# Copyright 2012, Richard Lowe.

include $(SRC)/Makefile.master

CFLAGS = 	-O1 -m64

LINK.c = env LD_ALTEXEC=$(PROTO)/usr/bin/amd64/ld.sun $(CC) $(CFLAGS) -o $@ $^
COMPILE.c = $(CC) $(CFLAGS) -c -o $@ $^
COMPILE.s = $(CC) $(CFLAGS) -c -o $@ $^

.KEEP_STATE:

install default: all

.c.o:
	$(COMPILE.c)

.s.o:
	$(COMPILE.s)

# A basic use of TLS that uses the movq m/r --> movq i/r variant
PROGS += 	style2
STYLE2OBJS =	style2.o
style2: $(STYLE2OBJS)
	$(LINK.c)

# A copy of style2 that uses %r13 in the TLS sequence, and thus excercises the
# REX transitions of the movq mem,reg -> movq imm,reg variant.
PROGS +=	style2-with-r13
STYLE2R13OBJS =	style2-with-r13.o
style2-with-r13: $(STYLE2R13OBJS)
	$(LINK.c)

# A copy of style2 that uses %r12 in the TLS sequence, so we can verify that
# it is _not_ special to this variant
PROGS +=	style2-with-r12
STYLE2R12OBJS =	style2-with-r12.o
style2-with-r12: $(STYLE2R12OBJS)
	$(LINK.c)

# A copy of style2 that has a R_AMD64_GOTTPOFF relocation with a bad insn sequence
STYLE2BADNESSOBJS =	style2-with-badness.o
style2-with-badness: $(STYLE2BADNESSOBJS)
	-$(LINK.c)

# A basic use of TLS that uses the addq mem/reg --> leaq mem,reg variant
PROGS +=	style1
STYLE1OBJS =	style1-main.o style1-func.o
style1: $(STYLE1OBJS)
	$(LINK.c)

# A copy of style1-func that uses %r13 in the TLS sequence and thus excercises
# the REX transitions. of the addq mem,reg --> leaq mem,reg variant
PROGS +=	style1-with-r13
STYLE1R13OBJS =	style1-main.o style1-func-with-r13.o
style1-with-r13: $(STYLE1R13OBJS)
	$(LINK.c)

# A copy of style1-func that uses %r12 to test the addq mem,reg --> addq imm,reg variant
PROGS +=	style1-with-r12
STYLE1R12OBJS =	style1-main.o style1-func-with-r12.o
style1-with-r12: $(STYLE1R12OBJS)
	$(LINK.c)

all:	$(PROGS)

clobber clean: 
	rm -f $(PROGS) $(STYLE1OBJS) $(STYLE1R13OBJS) $(STYLE1R12OBJS) \
	    $(STYLE2OBJS) $(STYLE2R13OBJS) $(STYLE2R12OBJS) $(STYLE2BADNESSOBJS)

fail: style2-with-badness FRC

FRC:
