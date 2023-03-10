# $Id: lib.mk,v 1.61 2017/05/06 17:30:09 sjg Exp $

.if !target(__${.PARSEFILE}__)
__${.PARSEFILE}__:

.include <init.mk>

.if ${OBJECT_FMT} == "ELF"
NEED_SOLINKS?= yes
.endif

SHLIB_VERSION_FILE?= ${.CURDIR}/shlib_version
.if !defined(SHLIB_MAJOR) && exists(${SHLIB_VERSION_FILE})
.include "${SHLIB_VERSION_FILE}"
SHLIB_MAJOR:=${major}
SHLIB_MINOR:=${minor}
.endif

.for x in major minor teeny
print-shlib-$x:
.if defined(SHLIB_${x:tu}) && ${MK_PIC} != "no"
	@echo ${SHLIB_${x:tu}}
.else
	@false
.endif
.endfor

.ifndef SHLIB_FULLVERSION
SHLIB_FULLVERSION= ${SHLIB_MAJOR}
.if !empty(SHLIB_MINOR)
SHLIB_FULLVERSION+= ${SHLIB_MINOR}
.endif
SHLIB_FULLVERSION := ${SHLIB_FULLVERSION:ts.}
.endif

# add additional suffixes not exported.
# .po is used for profiling object files.
# ${PICO} is used for PIC object files.
PICO?= .pico
.SUFFIXES: .out .a .ln ${PICO} .po .o .s .S .c .cc .C .m .F .f .r .y .l .cl .p .h
.SUFFIXES: .sh .m4 .m

CFLAGS+=	${COPTS}

# Originally derrived from NetBSD-1.6

# Set PICFLAGS to cc flags for producing position-independent code,
# if not already set.  Includes -DPIC, if required.

# Data-driven table using make variables to control how shared libraries
# are built for different platforms and object formats.
# OBJECT_FMT:		currently either "ELF" or "a.out", from <bsd.own.mk>
# SHLIB_SOVERSION:	version number to be compiled into a shared library
#			via -soname. Usually ${SHLIB_MAJOR} on ELF.
#			NetBSD/pmax used to use ${SHLIB_MAJOR}[.${SHLIB_MINOR}
# SHLIB_SHFLAGS:	Flags to tell ${LD} to emit shared library.
#			with ELF, also set shared-lib version for ld.so.
# SHLIB_LDSTARTFILE:	support .o file, call C++ file-level constructors
# SHLIB_LDENDFILE:	support .o file, call C++ file-level destructors
# FPICFLAGS:		flags for ${FC} to compile .[fF] files to ${PICO} objects.
# CPPICFLAGS:		flags for ${CPP} to preprocess .[sS] files for ${AS}
# CPICFLAGS:		flags for ${CC} to compile .[cC] files to ${PICO} objects.
# CAPICFLAGS		flags for {$CC} to compiling .[Ss] files
#		 	(usually just ${CPPPICFLAGS} ${CPICFLAGS})
# APICFLAGS:		flags for ${AS} to assemble .[sS] to ${PICO} objects.

MKPICLIB?= yes

# sys.mk can override these
LD_X?=-X
LD_x?=-x
LD_r?=-r

LD_shared=-shared -Wl,--soname,lib${LIB}.so.${SHLIB_FULLVERSION}

.if !empty(SHLIB_MAJOR)
.if ${NEED_SOLINKS} && empty(SHLIB_LINKS)
.if ${MK_LINKLIB} != "no"
SHLIB_LINKS = lib${LIB}.${LD_solink}
.endif
.if "${SHLIB_FULLVERSION}" != "${SHLIB_MAJOR}"
SHLIB_LINKS += lib${LIB}.${LD_solink}.${SHLIB_MAJOR}
.endif
.endif
.endif

LIBTOOL?=libtool
LD_so ?= so.${SHLIB_FULLVERSION}
LD_solink ?= so
.if empty(LORDER)
LD_objs ?= ${OBJS}
LD_pobjs ?= ${POBJS}
LD_sobjs ?= ${SOBJS}
.else
LD_objs ?= `${LORDER} ${OBJS} | ${TSORT}`
LD_sobjs ?= `${LORDER} ${SOBJS} | ${TSORT}`
LD_pobjs ?= `${LORDER} ${POBJS} | ${TSORT}`
.endif
LD_solib ?= ${LD_sobjs}
AR_cq ?= cq

# some libs have lots of objects, and scanning all .o, .po and ${PICO} meta files
# is a waste of time, this tells meta.autodep.mk to just pick one 
# (typically ${PICO})
# yes, 42 is a random number.
.if ${MK_DIRDEPS_BUILD} == "yes" && ${SRCS:Uno:[\#]} > 42
OPTIMIZE_OBJECT_META_FILES ?= yes
.endif

.if ${MK_LIBTOOL} == "yes"
# because libtool is so fascist about naming the object files,
# we cannot (yet) build profiled libs
MK_PROFILE=no
_LIBS=lib${LIB}.a
.if exists(${.CURDIR}/shlib_version)
SHLIB_AGE != . ${.CURDIR}/shlib_version ; echo $$age
.endif
.else
# for the normal .a we do not want to strip symbols
.c.o:
	${COMPILE.c} ${.IMPSRC}

# for the normal .a we do not want to strip symbols
${CXX_SUFFIXES:%=%.o}:
	${COMPILE.cc} ${.IMPSRC}

.S.o .s.o:
	@echo ${COMPILE.S} ${CFLAGS:M-[ID]*} ${AINC} ${.IMPSRC}
	@${COMPILE.S} ${CFLAGS:M-[ID]*} ${AINC} ${.IMPSRC} 

.if (${LD_X} == "")
.c.po:
	${COMPILE.c} ${CC_PG} ${PROFFLAGS} ${.IMPSRC} -o ${.TARGET}

${CXX_SUFFIXES:%=%.po}:
	${COMPILE.cc} -pg ${.IMPSRC} -o ${.TARGET}

.S${PICO} .s${PICO}:
	${COMPILE.S} ${PICFLAG} ${CC_PIC} ${CFLAGS:M-[ID]*} ${AINC} ${.IMPSRC} -o ${.TARGET}
.else
.c.po:
	@echo ${COMPILE.c} ${CC_PG} ${PROFFLAGS} ${.IMPSRC} -o ${.TARGET}
	@${COMPILE.c} ${CC_PG} ${PROFFLAGS} ${.IMPSRC} -o ${.TARGET}.o
	@${LD} ${LD_X} ${LD_r} ${.TARGET}.o -o ${.TARGET}
	@rm -f ${.TARGET}.o

${CXX_SUFFIXES:%=%.po}:
	@echo ${COMPILE.cc} ${CXX_PG} ${PROFFLAGS} ${.IMPSRC} -o ${.TARGET}
	@${COMPILE.cc} ${CXX_PG} ${.IMPSRC} -o ${.TARGET}.o
	@${LD} ${LD_X} ${LD_r} ${.TARGET}.o -o ${.TARGET}
	@rm -f ${.TARGET}.o

.S${PICO} .s${PICO}:
	@echo ${COMPILE.S} ${PICFLAG} ${CC_PIC} ${CFLAGS:M-[ID]*} ${AINC} ${.IMPSRC} -o ${.TARGET}
	@${COMPILE.S} ${PICFLAG} ${CC_PIC} ${CFLAGS:M-[ID]*} ${AINC} ${.IMPSRC} -o ${.TARGET}.o
	@${LD} ${LD_x} ${LD_r} ${.TARGET}.o -o ${.TARGET}
	@rm -f ${.TARGET}.o
.endif

.if (${LD_x} == "")
.c${PICO}:
	${COMPILE.c} ${PICFLAG} ${CC_PIC} ${.IMPSRC} -o ${.TARGET}

${CXX_SUFFIXES:%=%${PICO}}:
	${COMPILE.cc} ${PICFLAG} ${CC_PIC} ${.IMPSRC} -o ${.TARGET}

.S.po .s.po:
	${COMPILE.S} ${PROFFLAGS} ${CFLAGS:M-[ID]*} ${AINC} ${.IMPSRC} -o ${.TARGET}
.else

.c${PICO}:
	@echo ${COMPILE.c} ${PICFLAG} ${CC_PIC} ${.IMPSRC} -o ${.TARGET}
	@${COMPILE.c} ${PICFLAG} ${CC_PIC} ${.IMPSRC} -o ${.TARGET}.o
	@${LD} ${LD_x} ${LD_r} ${.TARGET}.o -o ${.TARGET}
	@rm -f ${.TARGET}.o

${CXX_SUFFIXES:%=%${PICO}}:
	@echo ${COMPILE.cc} ${PICFLAG} ${CC_PIC} ${.IMPSRC} -o ${.TARGET}
	@${COMPILE.cc} ${PICFLAG} ${CC_PIC} ${.IMPSRC} -o ${.TARGET}.o
	@${LD} ${LD_x} ${LD_r} ${.TARGET}.o -o ${.TARGET}
	@rm -f ${.TARGET}.o

.S.po .s.po:
	@echo ${COMPILE.S} ${PROFFLAGS} ${CFLAGS:M-[ID]*} ${AINC} ${.IMPSRC} -o ${.TARGET}
	@${COMPILE.S} ${PROFFLAGS} ${CFLAGS:M-[ID]*} ${AINC} ${.IMPSRC} -o ${.TARGET}.o
	@${LD} ${LD_X} ${LD_r} ${.TARGET}.o -o ${.TARGET}
	@rm -f ${.TARGET}.o

.endif
.endif

.if ${MK_LIBTOOL} != "yes"

.if !defined(PICFLAG)
PICFLAG=-fpic
.endif

_LIBS=

.if ${MK_ARCHIVE} != "no"
_LIBS += lib${LIB}.a
.endif

.if ${MK_PROFILE} != "no"
_LIBS+=lib${LIB}_p.a
POBJS+=${OBJS:.o=.po}
.endif

.if ${MK_PIC} != "no"
.if ${MK_PICLIB} == "no"
SOLIB ?= lib${LIB}.a
.else
SOLIB=lib${LIB}_pic.a
_LIBS+=${SOLIB}
.endif
.if !empty(SHLIB_FULLVERSION)
_LIBS+=lib${LIB}.${LD_so}
.endif
.endif

# here is where you can define what LIB* are
.-include <libnames.mk>
.if ${MK_DPADD_MK} == "yes"
# lots of cool magic, but might not suit everyone.
.include <dpadd.mk>
.endif

.if empty(LIB)
_LIBS=
.endif

.if !defined(_SKIP_BUILD)
realbuild: ${_LIBS} 
.endif

all: _SUBDIRUSE

.for s in ${SRCS:N*.h:M*/*}
${.o ${PICO} .po .lo:L:@o@${s:T:R}$o@}: $s
.endfor

OBJS+=	${SRCS:T:N*.h:R:S/$/.o/g}
.NOPATH:	${OBJS}

.if ${MK_LIBTOOL} == "yes"
.if ${MK_PIC} == "no"
LT_STATIC=-static
.else
LT_STATIC=
.endif
SHLIB_AGE?=0

# .lo's are created as a side effect
.s.o .S.o .c.o:
	${LIBTOOL} --mode=compile ${CC} ${LT_STATIC} ${CFLAGS} ${CPPFLAGS} ${IMPFLAGS} -c ${.IMPSRC}

# can't really do profiled libs with libtool - its too fascist about
# naming the output...
lib${LIB}.a:: ${OBJS}
	@rm -f ${.TARGET}
	${LIBTOOL} --mode=link ${CC} ${LT_STATIC} -o ${.TARGET:.a=.la} ${OBJS:.o=.lo} -rpath ${SHLIBDIR}:/usr/lib -version-info ${SHLIB_MAJOR}:${SHLIB_MINOR}:${SHLIB_AGE}
	@ln .libs/${.TARGET} .

lib${LIB}.${LD_so}:: lib${LIB}.a
	@[ -s ${.TARGET}.${SHLIB_AGE} ] || { ln -s .libs/lib${LIB}.${LD_so}* . 2>/dev/null; : }
	@[ -s ${.TARGET} ] || ln -s ${.TARGET}.${SHLIB_AGE} ${.TARGET}

.else  # MK_LIBTOOL=yes

lib${LIB}.a:: ${OBJS}
	@echo building standard ${LIB} library
	@rm -f ${.TARGET}
	@${AR} ${AR_cq} ${.TARGET} ${LD_objs}

POBJS+=	${OBJS:.o=.po}
.NOPATH:	${POBJS}
lib${LIB}_p.a:: ${POBJS}
	@echo building profiled ${LIB} library
	@rm -f ${.TARGET}
	@${AR} ${AR_cq} ${.TARGET} ${LD_pobjs}

SOBJS+=	${OBJS:.o=${PICO}}
.NOPATH:	${SOBJS}
lib${LIB}_pic.a:: ${SOBJS}
	@echo building shared object ${LIB} library
	@rm -f ${.TARGET}
	@${AR} ${AR_cq} ${.TARGET} ${LD_sobjs}

#SHLIB_LDADD?= ${LDADD}

.if defined(VERSION_SCRIPT)
SHLIB_LDADD+=	-Wl,--version-script=${VERSION_SCRIPT}
.endif

lib${LIB}.${LD_so}: ${SOBJS} ${DPADD} ${VERSION_SCRIPT}
	@echo building shared ${LIB} library \(version ${SHLIB_FULLVERSION}\)
	@rm -f ${.TARGET}
	${CC} ${CFLAGS} -o ${.TARGET} ${LD_shared} ${LD_solib} ${DLLIB} ${SHLIB_LDADD} -lc
.endif
.if !empty(SHLIB_LINKS)
	rm -f ${SHLIB_LINKS}; ${SHLIB_LINKS:O:u:@x@ln -s ${.TARGET} $x;@}
.endif

.if !target(clean)
cleanlib: .PHONY
	rm -f a.out [Ee]rrs mklog core *.core ${CLEANFILES}
	rm -f lib${LIB}.a ${OBJS}
	rm -f lib${LIB}_p.a ${POBJS}
	rm -f lib${LIB}_pic.a lib${LIB}.so.* ${SOBJS}
.if !empty(SHLIB_LINKS)
	rm -f ${SHLIB_LINKS}
.endif

clean: _SUBDIRUSE cleanlib
cleandir: _SUBDIRUSE cleanlib
.else
cleandir: _SUBDIRUSE clean
.endif

.if defined(SRCS) && (!defined(MKDEP) || ${MKDEP} != autodep)
afterdepend: .depend
	@(TMP=/tmp/_depend$$$$; \
	    sed -e 's/^\([^\.]*\).o[ ]*:/\1.o \1.po \1${PICO} \1.ln:/' \
	      < .depend > $$TMP; \
	    mv $$TMP .depend)
.endif

.if !target(install)
.if !target(beforeinstall)
beforeinstall:
.endif

.if !empty(LIBOWN)
LIB_INSTALL_OWN ?= -o ${LIBOWN} -g ${LIBGRP}
.endif

.include <links.mk>

.if !target(realinstall) && !empty(LIB)
realinstall: libinstall
.endif
.if !target(libinstall)
libinstall:
	${INSTALL} -d ${LIB_INSTALL_OWN} -m 755 ${DESTDIR}${LIBDIR}
.if ${MK_ARCHIVE} != "no"
	${INSTALL} ${COPY} ${LIB_INSTALL_OWN} -m 600 lib${LIB}.a \
	    ${DESTDIR}${LIBDIR}
	chmod ${LIBMODE} ${DESTDIR}${LIBDIR}/lib${LIB}.a
.endif
.if ${MK_PROFILE} != "no"
	${INSTALL} ${COPY} ${LIB_INSTALL_OWN} -m 600 \
	    lib${LIB}_p.a ${DESTDIR}${LIBDIR}
	chmod ${LIBMODE} ${DESTDIR}${LIBDIR}/lib${LIB}_p.a
.endif
.if ${MK_PIC} != "no"
.if ${MK_PICLIB} != "no"
	${INSTALL} ${COPY} ${LIB_INSTALL_OWN} -m 600 \
	    lib${LIB}_pic.a ${DESTDIR}${LIBDIR}
	chmod ${LIBMODE} ${DESTDIR}${LIBDIR}/lib${LIB}_pic.a
.endif
.if !empty(SHLIB_MAJOR)
	${INSTALL} ${COPY} ${LIB_INSTALL_OWN} -m ${LIBMODE} \
	    lib${LIB}.${LD_so} ${DESTDIR}${LIBDIR}
.if !empty(SHLIB_LINKS)
	(cd ${DESTDIR}${LIBDIR} && { rm -f ${SHLIB_LINKS}; ${SHLIB_LINKS:O:u:@x@ln -s lib${LIB}.${LD_so} $x;@} })
.endif
.endif
.endif
.if defined(SYMLINKS) && !empty(SYMLINKS)
	@set ${SYMLINKS}; ${_SYMLINKS_SCRIPT}
.endif
.if defined(LINKS) && !empty(LINKS)
	@set ${LINKS}; ${_LINKS_SCRIPT}
.endif
.endif

.if ${MK_MAN} != "no"
install: maninstall _SUBDIRUSE
maninstall: afterinstall
.endif
afterinstall: realinstall
libinstall: beforeinstall
realinstall: beforeinstall
.endif

.if defined(FILES) || defined(FILESGROUPS)
.include <files.mk>
.endif

.if ${MK_MAN} != "no"
.include <man.mk>
.endif

.if ${MK_NLS} != "no"
.include <nls.mk>
.endif

BUILDAFTER+= ${_LIBS} ${OBJS} ${POBJS} ${SOBJS}

.include <obj.mk>
.include <inc.mk>
.include <dep.mk>
.include <subdir.mk>
.endif

# during building we usually need/want to install libs somewhere central
# note that we do NOT ch{own,grp} as that would likely fail at this point.
# otherwise it is the same as realinstall
# Note that we don't need this when using dpadd.mk
.libinstall:	${_LIBS}
	${INSTALL} -d -m755 ${DESTDIR}${LIBDIR}
.for _lib in ${_LIBS:M*.a}
	${INSTALL} ${COPY} -m 644 ${_lib} ${DESTDIR}${LIBDIR}
.endfor
.for _lib in ${_LIBS:M*.${LD_solink}*:O:u}
	${INSTALL} ${COPY} -m ${LIBMODE} ${_lib} ${DESTDIR}${LIBDIR}
.if !empty(SHLIB_LINKS)
	(cd ${DESTDIR}${LIBDIR} && { ${SHLIB_LINKS:O:u:@x@ln -sf ${_lib} $x;@}; })
.endif
.endfor
	@touch ${.TARGET}

.if !empty(LIB)
STAGE_LIBDIR?= ${STAGE_OBJTOP}${LIBDIR}
stage_libs: ${_LIBS}
.endif

.include <final.mk>
.endif
