MODULE=		stmf_sbd
MODULE_TYPE=	drv
MODULE_DEPS=	drv/stmf \
		fs/zfs
MODULE_CONF=	stmf_sbd.conf
INCS=		${SRCTOP}/kernel/fs/zfs
SRCS=		sbd.c \
		sbd_pgr.c \
		sbd_scsi.c \
		sbd_zvol.c

CERRWARN =  -Wno-switch
CERRWARN += -Wno-parentheses
CERRWARN += -Wno-unused-label
CERRWARN += -Wno-uninitialized

.include <kmod.mk>
