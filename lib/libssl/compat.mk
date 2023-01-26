.include <unleashed.mk>
LCRYPTO_SRC=	${SRCTOP}/lib/libcrypto
CPPFLAGS+=	-D__BEGIN_HIDDEN_DECLS= -D__END_HIDDEN_DECLS=
CPPFLAGS+=	-I${LCRYPTO_SRC}/compat/include
SRCS+=		timingsafe_memcmp.c
.PATH:		${LCRYPTO_SRC}/compat

BSDOBJDIR?=     ${.OBJDIR:tA:H:H}
SHLIB_LDADD?=   ${LDADD}

INSTALL_COPY?=	${COPY}
SHAREMODE?=	444

# XXX <openssl/*> includes need to be available in DESTDIR if DESTDIR is
# specified since the mk files set -isysroot in that case
.ifdef DESTDIR
beforebuild: includes
# 'includes' assumes usr/include exists
includes: mkincdir
mkincdir:
	${INSTALL} -d ${DESTDIR}/usr/include
.endif
