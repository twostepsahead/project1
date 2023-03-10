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
# Copyright 2016 Hans Rosenfeld <rosenfeld@grumpf.hope-2000.org>
#

#
# The default for payload-bearing actions in this package is to appear in the
# global zone only.  See the include file for greater detail, as well as
# information about overriding the defaults.
#
dir path=kernel group=sys
dir path=kernel/drv group=sys
dir path=kernel/firmware group=sys
dir path=kernel/firmware/iwn group=sys
dir path=usr/share/man
dir path=usr/share/man/man7d
driver name=iwn clone_perms="iwn 0666 root sys" perms="* 0666 root sys" \
    alias=pciex8086,4229 \
    alias=pciex8086,422b \
    alias=pciex8086,422c \
    alias=pciex8086,422d \
    alias=pciex8086,4230 \
    alias=pciex8086,4232 \
    alias=pciex8086,4233 \
    alias=pciex8086,4235 \
    alias=pciex8086,4236 \
    alias=pciex8086,4237 \
    alias=pciex8086,4238 \
    alias=pciex8086,4239 \
    alias=pciex8086,423a \
    alias=pciex8086,423b \
    alias=pciex8086,423c \
    alias=pciex8086,423d \
    alias=pciex8086,82 \
    alias=pciex8086,83 \
    alias=pciex8086,84 \
    alias=pciex8086,85 \
    alias=pciex8086,87 \
    alias=pciex8086,887 \
    alias=pciex8086,888 \
    alias=pciex8086,88e \
    alias=pciex8086,88f \
    alias=pciex8086,89 \
    alias=pciex8086,890 \
    alias=pciex8086,891 \
    alias=pciex8086,892 \
    alias=pciex8086,893 \
    alias=pciex8086,894 \
    alias=pciex8086,895 \
    alias=pciex8086,896 \
    alias=pciex8086,897 \
    alias=pciex8086,8a \
    alias=pciex8086,8ae \
    alias=pciex8086,8af \
    alias=pciex8086,8b \
    alias=pciex8086,8d \
    alias=pciex8086,8e \
    alias=pciex8086,90 \
    alias=pciex8086,91
file path=kernel/drv/iwn group=sys
file path=kernel/firmware/iwn/iwlwifi-100-5.ucode
file path=kernel/firmware/iwn/iwlwifi-1000-3.ucode
file path=kernel/firmware/iwn/iwlwifi-105-6.ucode
file path=kernel/firmware/iwn/iwlwifi-135-6.ucode
file path=kernel/firmware/iwn/iwlwifi-2000-6.ucode
file path=kernel/firmware/iwn/iwlwifi-2030-6.ucode
file path=kernel/firmware/iwn/iwlwifi-4965-2.ucode
file path=kernel/firmware/iwn/iwlwifi-5000-2.ucode
file path=kernel/firmware/iwn/iwlwifi-5150-2.ucode
file path=kernel/firmware/iwn/iwlwifi-6000-4.ucode
file path=kernel/firmware/iwn/iwlwifi-6000g2a-6.ucode
file path=kernel/firmware/iwn/iwlwifi-6000g2b-6.ucode
file path=kernel/firmware/iwn/iwlwifi-6050-5.ucode
file path=usr/share/man/man7d/iwn.7d
license kernel/drivers/net/wifi/iwn/THIRDPARTYLICENSE \
    license=kernel/drivers/net/wifi/iwn/THIRDPARTYLICENSE
license kernel/drivers/net/wifi/iwn/fw/THIRDPARTYLICENSE \
    license=kernel/drivers/net/wifi/iwn/fw/THIRDPARTYLICENSE
license lic_CDDL license=lic_CDDL
