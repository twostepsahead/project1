#	$Id: Makefile,v 1.92 2017/05/10 22:29:04 sjg Exp $

# Base version on src date
_MAKE_VERSION= 20170510

PROG=	make

SRCS= \
	arch.c \
	buf.c \
	compat.c \
	cond.c \
	dir.c \
	for.c \
	hash.c \
	job.c \
	main.c \
	make.c \
	make_malloc.c \
	meta.c \
	metachar.c \
	parse.c \
	str.c \
	strlist.c \
	suff.c \
	targ.c \
	trace.c \
	util.c \
	var.c

# from lst.lib/
SRCS+= \
	lstAppend.c \
	lstAtEnd.c \
	lstAtFront.c \
	lstClose.c \
	lstConcat.c \
	lstDatum.c \
	lstDeQueue.c \
	lstDestroy.c \
	lstDupl.c \
	lstEnQueue.c \
	lstFind.c \
	lstFindFrom.c \
	lstFirst.c \
	lstForEach.c \
	lstForEachFrom.c \
	lstInit.c \
	lstInsert.c \
	lstIsAtEnd.c \
	lstIsEmpty.c \
	lstLast.c \
	lstMember.c \
	lstNext.c \
	lstOpen.c \
	lstPrev.c \
	lstRemove.c \
	lstReplace.c \
	lstSucc.c

LINKS=	${BINDIR}/make ${BINDIR}/bmake

# this file gets generated by configure
.-include "Makefile.config"

.if !empty(LIBOBJS)
SRCS+= ${LIBOBJS:T:.o=.c}
.endif

# just in case
prefix?= /usr
srcdir?= ${.CURDIR}

DEFAULT_SYS_PATH?= ${prefix}/share/mk

CPPFLAGS+= -DUSE_META
CFLAGS+= ${CPPFLAGS}
CFLAGS+= -D_PATH_DEFSYSPATH=\"${DEFAULT_SYS_PATH}\"
CFLAGS+= -I. -I${srcdir} ${XDEFS} -DMAKE_NATIVE
CFLAGS+= ${COPTS.${.ALLSRC:M*.c:T:u}}
COPTS.main.c+= "-DMAKE_VERSION=\"${_MAKE_VERSION}\""

# meta mode can be useful even without filemon 
FILEMON_H ?= /usr/include/dev/filemon/filemon.h
.if exists(${FILEMON_H}) && ${FILEMON_H:T} == "filemon.h"
COPTS.meta.c += -DHAVE_FILEMON_H -I${FILEMON_H:H}
.endif

.PATH:	${srcdir}
.PATH:	${srcdir}/lst.lib

.if make(obj) || make(clean)
SUBDIR+= unit-tests
.endif

MAN= ${PROG}.1
MAN1= ${MAN}

.if (${PROG} != "make")
CLEANFILES+= my.history
.if make(${MAN}) || !exists(${srcdir}/${MAN})
my.history: ${MAKEFILE}
	@(echo ".Nm"; \
	echo "is derived from NetBSD"; \
	echo ".Xr make 1 ."; \
	echo "It uses autoconf to facilitate portability to other platforms."; \
	echo ".Pp") > $@

.NOPATH: ${MAN}
${MAN}:	make.1 my.history
	@echo making $@
	@sed \
	-e '/^.Dt/s/MAKE/${PROG:tu}/' \
	-e 's/^.Nx/NetBSD/' \
	-e '/^.Nm/s/make/${PROG}/' \
	-e '/^.Sh HISTORY/rmy.history' \
	-e '/^.Sh HISTORY/,$$s,^.Nm,make,' ${srcdir}/make.1 > $@

all beforeinstall: ${MAN}
_mfromdir=.
.endif
.endif

MANTARGET?= cat
MANDEST?= ${MANDIR}/${MANTARGET}1

.if ${MANTARGET} == "cat"
_mfromdir=${srcdir}
.endif

.include <prog.mk>

CPPFLAGS+= -DMAKE_NATIVE -DHAVE_CONFIG_H
COPTS.var.c += -Wno-cast-qual
COPTS.job.c += -Wno-format-nonliteral
COPTS.parse.c += -Wno-format-nonliteral
COPTS.var.c += -Wno-format-nonliteral

# Force these
SHAREDIR= ${SHAREDIR.bmake:U${prefix}/share}
BINDIR= ${BINDIR.bmake:U${prefix}/bin}
MANDIR= ${MANDIR.bmake:U${SHAREDIR}/man}

.if !exists(.depend)
${OBJS}: config.h
.endif

# make sure that MAKE_VERSION gets updated.
main.o: ${SRCS} ${MAKEFILE}

# A simple unit-test driver to help catch regressions
accept test:
	cd ${.CURDIR}/unit-tests && MAKEFLAGS= ${.MAKE} -r -m / TEST_MAKE=${TEST_MAKE:U${.OBJDIR}/${PROG:T}} ${.TARGET}
