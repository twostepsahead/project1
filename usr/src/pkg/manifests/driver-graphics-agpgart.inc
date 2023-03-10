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
#

#
# The default for payload-bearing actions in this package is to appear in the
# global zone only.  See the include file for greater detail, as well as
# information about overriding the defaults.
#
dir path=kernel group=sys
dir path=kernel/drv group=sys
dir path=kernel/misc group=sys
dir path=usr/share/man
dir path=usr/share/man/man7i
driver name=agpgart perms="* 0644 root sys"
driver name=agptarget perms="* 0644 root sys" \
    alias=pci1022,7454 \
    alias=pci8086,1130 \
    alias=pci8086,2560 \
    alias=pci8086,2570 \
    alias=pci8086,2580 \
    alias=pci8086,2590 \
    alias=pci8086,2770 \
    alias=pci8086,27a0 \
    alias=pci8086,27ac \
    alias=pci8086,2970 \
    alias=pci8086,2980 \
    alias=pci8086,2990 \
    alias=pci8086,29a0 \
    alias=pci8086,29b0 \
    alias=pci8086,29c0 \
    alias=pci8086,29d0 \
    alias=pci8086,2a00 \
    alias=pci8086,2a10 \
    alias=pci8086,2a40 \
    alias=pci8086,2e00 \
    alias=pci8086,2e10 \
    alias=pci8086,2e20 \
    alias=pci8086,2e30 \
    alias=pci8086,2e40 \
    alias=pci8086,3575 \
    alias=pci8086,3580 \
    alias=pci8086,40 \
    alias=pci8086,44 \
    alias=pci8086,62 \
    alias=pci8086,6a \
    alias=pci8086,7120 \
    alias=pci8086,7122 \
    alias=pci8086,7124
# Note: the name of this driver _really_is_ amd64_gart...
driver name=amd64_gart alias=pci1022,1103 perms="* 0644 root sys"
file path=kernel/drv/$(ARCH64)_gart group=sys
file path=kernel/drv/agpgart group=sys
file path=kernel/drv/agpgart.conf group=sys
file path=kernel/drv/agptarget group=sys
file path=kernel/misc/agpmaster group=sys mode=0755
file path=usr/share/man/man7i/agpgart_io.7i
license cr_Sun license=cr_Sun
license include/sys/THIRDPARTYLICENSE.agpgart \
    license=include/sys/THIRDPARTYLICENSE.agpgart
license lic_CDDL license=lic_CDDL
