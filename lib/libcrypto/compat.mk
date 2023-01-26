MACHINE_CPU?=	${MACHINE}

CPPFLAGS+=	-D__BEGIN_HIDDEN_DECLS= -D__END_HIDDEN_DECLS=
CPPFLAGS+=	-I${LCRYPTO_SRC}/compat/include
# see libressl-portable/portable:m4/disable-compiler-warnings.m4 
CFLAGS+=	-Wno-pointer-sign
# sizeof(time_t) == 4 (2038 problem)
CFLAGS+=	-DSMALL_TIME_T

SRCS+=		compat/timingsafe_bcmp.c compat/timingsafe_memcmp.c
INSTALL_COPY?=	${COPY}
SHAREMODE?=	444

# XXX <openssl/*> includes need to be available in DESTDIR if DESTDIR is
# specified since the mk files set -isysroot in that case
.ifdef DESTDIR
beforebuild: includes prereq
# 'includes' assumes usr/include exists
includes: mkincdir
mkincdir:
	${INSTALL} -d ${DESTDIR}/usr/include
.endif
# install config files as well
afterinstall: distribution
distribution: mkdir_etc_ssl
mkdir_etc_ssl:
	${INSTALL} -d ${DESTDIR}/etc/ssl
