.include "${.CURDIR:H}/config.mk"

SRCDIR=	${NCURSES_DIR}/menu

LIB=	menu

.PATH: ${SRCDIR}
SRCS=	\
	ncurses_def.h \
	m_attribs.c \
	m_cursor.c \
	m_driver.c \
	m_format.c \
	m_global.c \
	m_hook.c \
	m_item_cur.c \
	m_item_nam.c \
	m_item_new.c \
	m_item_opt.c \
	m_item_top.c \
	m_item_use.c \
	m_item_val.c \
	m_item_vis.c \
	m_items.c \
	m_new.c \
	m_opts.c \
	m_pad.c \
	m_pattern.c \
	m_post.c \
	m_req_name.c \
	m_scale.c \
	m_spacing.c \
	m_sub.c \
	m_userptr.c \
	m_win.c

CLEANFILES=	ncurses_def.h

BSDOBJDIR?=     ${.OBJDIR:tA:H:H}
CFLAGS+=	-I${SRCDIR}
CFLAGS+=        -I${.CURDIR:H}/ncurses
CFLAGS+=	-I${BSDOBJDIR}/ncurses/ncurses

LDADD+= -L${BSDOBJDIR}/ncurses/ncurses -lncurses
SHLIB_LDADD+=  ${LDADD}

INCS=	${SRCDIR}/menu.h ${SRCDIR}/eti.h

includes: incinstall

pc_files:
	${INSTALL} ${INSTALL_COPY} -o ${SHAREOWN} -g ${SHAREGRP} \
		-m 0644 ${NCURSES_DIR}/menu/menu.pc ${DESTDIR}/usr/lib/pkgconfig/

VERSION_SCRIPT=   ${.CURDIR}/Symbols.map

.ifdef DESTDIR
afterinstall: includes pc_files
.endif

.include <bsd.lib.mk>
