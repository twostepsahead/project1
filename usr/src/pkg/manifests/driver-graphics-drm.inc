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
dir path=usr/lib
dir path=usr/lib/devfsadm group=sys
dir path=usr/lib/devfsadm/linkmod group=sys
dir path=usr/share/man
dir path=usr/share/man/man7d
driver name=i915 perms="* 0644 root sys" \
    alias=pci8086,2562 \
    alias=pci8086,2572 \
    alias=pci8086,2582 \
    alias=pci8086,2592 \
    alias=pci8086,2772 \
    alias=pci8086,27a2 \
    alias=pci8086,27ae \
    alias=pci8086,2972 \
    alias=pci8086,2982 \
    alias=pci8086,2992 \
    alias=pci8086,29a2 \
    alias=pci8086,29b2 \
    alias=pci8086,29c2 \
    alias=pci8086,29d2 \
    alias=pci8086,2a02 \
    alias=pci8086,2a12 \
    alias=pci8086,2a42 \
    alias=pci8086,2e02.8086.2e02 \
    alias=pci8086,2e12 \
    alias=pci8086,2e22 \
    alias=pci8086,2e32 \
    alias=pci8086,2e42 \
    alias=pci8086,42 \
    alias=pci8086,46
file path=kernel/drv/i915 group=sys
file path=kernel/drv/radeon group=sys
file path=kernel/misc/drm group=sys mode=0755
file path=usr/lib/devfsadm/linkmod/SUNW_drm_link_i386.so group=sys
file path=usr/share/man/man7d/i915.7d
file path=usr/share/man/man7d/radeon.7d
license cr_Sun license=cr_Sun
license usr/src/uts/common/io/drm/THIRDPARTYLICENSE \
    license=usr/src/uts/common/io/drm/THIRDPARTYLICENSE
