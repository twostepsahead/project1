#
# CDDL HEADER START
#
# The contents of this file are subject to the terms of the
# Common Development and Distribution License (the "License").
# You may not use this file except in compliance with the License.
#
# You can obtain a copy of the license at usr/src/OPENSOLARIS.LICENSE
# or http://www.opensolaris.org/os/licensing.
# See the License for the specific language governing permissions
# and limitations under the License.
#
# When distributing Covered Code, include this CDDL HEADER in each
# file and include the License file at usr/src/OPENSOLARIS.LICENSE.
# If applicable, add the following below this CDDL HEADER, with the
# fields enclosed by brackets "[]" replaced with your own identifying
# information: Portions Copyright [yyyy] [name of copyright owner]
#
# CDDL HEADER END
#

#
# Copyright (c) 2010, Oracle and/or its affiliates. All rights reserved.
# Copyright 2011 Nexenta Systems, Inc.  All rights reserved.
#

#
# The default for payload-bearing actions in this package is to appear in the
# global zone only.  See the include file for greater detail, as well as
# information about overriding the defaults.
#
dir path=kernel group=sys
dir path=kernel/drv group=sys
dir path=kernel/misc group=sys
dir path=kernel/misc/qlc group=sys
dir path=usr/share/man
dir path=usr/share/man/man7d
driver name=qlc class=fibre-channel perms="* 0666 root sys" \
    alias=pci1077,2200 \
    alias=pci1077,2300 \
    alias=pci1077,2312 \
    alias=pci1077,2322 \
    alias=pci1077,2422 \
    alias=pciex1077,2432 \
    alias=pciex1077,2532 \
    alias=pciex1077,5432 \
    alias=pciex1077,8001 \
    alias=pciex1077,8021
driver name=qlge alias=pciex1077,8000 perms="* 0666 root sys"
file path=kernel/drv/qlc group=sys
file path=kernel/drv/qlc.conf group=sys preserve=true
file path=kernel/drv/qlge group=sys
file path=kernel/misc/qlc/qlc_fw_2200 group=sys mode=0755
file path=kernel/misc/qlc/qlc_fw_2300 group=sys mode=0755
file path=kernel/misc/qlc/qlc_fw_2400 group=sys mode=0755
file path=kernel/misc/qlc/qlc_fw_2500 group=sys mode=0755
file path=kernel/misc/qlc/qlc_fw_6322 group=sys mode=0755
file path=kernel/misc/qlc/qlc_fw_8100 group=sys mode=0755
file path=usr/share/man/man7d/qlc.7d
license cr_Sun license=cr_Sun
license lic_CDDL license=lic_CDDL
license usr/src/uts/common/io/fibre-channel/fca/qlc/FIRMWARELICENSE \
    license=usr/src/uts/common/io/fibre-channel/fca/qlc/FIRMWARELICENSE