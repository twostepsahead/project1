SUBDIR = bin \
	 boot \
	 etc \
	 include \
	 kernel \
	 lib \
	 libexec \
	 share

.if !make(build) && !make(gen-config)
.if !exists(${.CURDIR}/cfgparam.mk)
.error run make gen-config to generate cfgparam.mk
.endif
.include "cfgparam.mk"
.endif

.if make(build) && !defined(DESTDIR)
.MAKEFLAGS+= DESTDIR=${.CURDIR}/proto/root_i386
.endif
build:: tools gen-config
	${.MAKE} obj
	${.MAKE} -C include # kernel/ depends on these; build separately
	${.MAKE}
	${.MAKE} install
	${.MAKE} -C lib build32 # special multiarch target
	${.MAKE} -C usr install # dmake expects libs to already be in DESTDIR
	${.MAKE} -C tools/postbuild clean obj
	${.MAKE} -C tools/postbuild

.include <unleashed.mk>
.include <subdir.mk>

cleandir: clean_artifacts clean_tools clean_lib
clean_artifacts::
	rm -rf ${.CURDIR}/proto/root_i386 ${.CURDIR}/packages/i386/nightly/repo.redist
# tools/ is not in SUBDIR
clean_tools::
	${.MAKE} -C tools cleandir
clean_lib::
	${.MAKE} -C lib cleanboth
# FIXME pretty dumb, but we need cfgparam.mk to clean some subdirs. this is
# because eg. include/ has additional subdirs based on some config. so do not
# remove gen-config outputs even if cleandir is specified, for now.
#clean_cfgparam::
#	rm -f ${.CURDIR}/usr/src/Makefile.cfgparam ${.CURDIR}/include/sys/cfgparam.h

#
# Config related support
#

.if !empty(BUILD_ARCH)
CFGARCH=${BUILD_ARCH}
.elif ${MACHINE} == "i86pc" || ${MACHINE} == "i386" || ${MACHINE} == "amd64"
CFGARCH=x86
.elif ${MACHINE} == "sparc"
CFGARCH=sparc
.else
.error "Unknown machine architecture ${MACHINE}; override it via BUILD_ARCH"
.endif

CFGFILE=arch/${CFGARCH}/Sconfig

tools::
	${.MAKE} -C tools obj
	${.MAKE} -C tools

${.CURDIR}/include/sys/cfgparam.h: tools
	${.CURDIR}/tools/mkconfig/obj/mkconfig -I _SYS_CFGPARAM_H -H -o $@ ${CFGFILE}
${.CURDIR}/usr/src/Makefile.cfgparam: tools
	${.CURDIR}/tools/mkconfig/obj/mkconfig -m -o $@ ${CFGFILE}
${.CURDIR}/cfgparam.mk: tools
	${.CURDIR}/tools/mkconfig/obj/mkconfig -M -o $@ ${CFGFILE}
gen-config:: ${.CURDIR}/cfgparam.mk ${.CURDIR}/include/sys/cfgparam.h ${.CURDIR}/usr/src/Makefile.cfgparam
