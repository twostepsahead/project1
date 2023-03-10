#
# This file and its contents are supplied under the terms of the
# Common Development and Distribution License ("CDDL"), version 1.0.
# You may only use this file in accordance with the terms of version
# 1.0 of the CDDL.
#
# A full copy of the text of the CDDL should have accompanied this
# source.  A copy of the CDDL is also available via the Internet at
# http://www.illumos.org/license/CDDL.
#

#
# Copyright (c) 2017 Joyent, Inc.
#

dir path=kernel group=sys
dir path=kernel/drv group=sys
dir path=usr/share/man
dir path=usr/share/man/man7d
driver name=i40e clone_perms="i40e 0666 root sys" perms="* 0666 root sys" \
    alias=pciex8086,1572 \
    alias=pciex8086,1580 \
    alias=pciex8086,1581 \
    alias=pciex8086,1583 \
    alias=pciex8086,1584 \
    alias=pciex8086,1585 \
    alias=pciex8086,1586 \
    alias=pciex8086,1589 \
    alias=pciex8086,158a \
    alias=pciex8086,158b \
    alias=pciex8086,37ce \
    alias=pciex8086,37cf \
    alias=pciex8086,37d0 \
    alias=pciex8086,37d1 \
    alias=pciex8086,37d2 \
    alias=pciex8086,37d3
file path=kernel/drv/i40e group=sys
file path=kernel/drv/i40e.conf group=sys
file path=usr/share/man/man7d/i40e.7d
license kernel/drivers/net/i40e/core/THIRDPARTYLICENSE \
    license=kernel/drivers/net/i40e/core/THIRDPARTYLICENSE
license lic_CDDL license=lic_CDDL
