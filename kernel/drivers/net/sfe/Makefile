MODULE=		sfe
MODULE_TYPE=	drv
MODULE_DEPS=	misc/mac \
		drv/ip
SRCS=		sfe.c \
		sfe_util.c
DEFS_amd64=	-Di86pc
DEFS_sparc=	-Dsun4u
DEFS=		-DVERSION='"2.6.1"' \
		${DEFS_${CONFIG_MACH}} \
		-D"__INLINE__=" \
		-DGEM_CONFIG_POLLING -DGEM_CONFIG_GLDv3 -DGEM_CONFIG_VLAN \
		-DGEM_CONFIG_CKSUM_OFFLOAD -DGEM_CONFIG_ND \
		-DCONFIG_DP83815 -DCONFIG_SIS900 -DCONFIG_SIS7016 \
		-DCONFIG_MAC_ADDR_SIS630E -DCONFIG_OPT_IO -UCONFIG_OO \
		-DCONFIG_PATTERN_MATCH_DP83815

CERRWARN =  -Wno-switch
CERRWARN += -Wno-parentheses
CERRWARN += -Wno-uninitialized
CERRWARN += -Wno-unused-label

.include <kmod.mk>
