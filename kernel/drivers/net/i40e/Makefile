MODULE=		i40e
MODULE_TYPE=	drv
MODULE_CONF=	i40e.conf
MODULE_DEPS=	misc/mac
SRCS=		i40e_gld.c \
		i40e_intr.c \
		i40e_main.c \
		i40e_osdep.c \
		i40e_stats.c \
		i40e_transceiver.c
INCS=		${.CURDIR}

SRCS+=		core/i40e_adminq.c \
		core/i40e_common.c \
		core/i40e_hmc.c \
		core/i40e_lan_hmc.c \
		core/i40e_nvm.c
INCS+=		${.CURDIR}/core

.include <kmod.mk>
