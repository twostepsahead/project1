.include <unleashed.mk>

NCURSES_DIR=${SRCTOP}/contrib/ncurses
NCURSES_INC=${NCURSES_DIR}/include
NCURSES_SRC=${NCURSES_DIR}/ncurses
NCURSES_TINFO=${NCURSES_SRC}/tinfo
NCURSES_SERIAL=${NCURSES_SRC}/tty
NCURSES_BASE=${NCURSES_SRC}/base
NCURSES_TRACE=${NCURSES_SRC}/trace
NCURSES_WIDE=${NCURSES_SRC}/widechar

BSDOBJDIR?=     ${.OBJDIR:tA:H:H}

TERMINFO        = /usr/share/terminfo
TERMINFO_CAPS=  ${NCURSES_INC}/Caps
TERMINFO_SRC    = ${NCURSES_DIR}/misc/terminfo.src
TIC_PATH        = /usr/bin/tic

SHLIB_MAJOR=	6
SHLIB_MINOR=	0

AWK		= awk

USE_BIG_STRINGS = 1
ENABLE_WIDEC=   1

DBG=

CFLAGS+=	-Wall --param max-inline-insns-single=1200
CFLAGS+=	-DHAVE_CONFIG_H -D__EXTENSIONS__ -DNDEBUG
CFLAGS+=	-D_XOPEN_SOURCE_EXTENDED
CFLAGS+=	-DENABLE_WIDEC
CFLAGS+=	-I${NCURSES_INC}
CFLAGS+=	-I${NCURSES_SRC} 

.PATH: ${NCURSES_INC}
.PATH: ${NCURSES_SRC}
.PATH: ${NCURSES_TINFO}
.PATH: ${NCURSES_BASE}
.PATH: ${NCURSES_SERIAL}
.PATH: ${NCURSES_TRACE}
.PATH: ${NCURSES_WIDE}