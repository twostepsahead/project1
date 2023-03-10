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
dir path=usr/share/man
dir path=usr/share/man/man7d
driver name=rwn clone_perms="rwn 0666 root sys" perms="* 0666 root sys" \
    alias=pci1814,601 \
    alias=pci1814,681 \
    alias=pci1814,701 \
    alias=pci1814,781 \
    alias=pci1a3b,1059
file path=kernel/drv/rwn group=sys
file path=usr/share/man/man7d/rwn.7d
license kernel/drivers/net/wifi/rwn/THIRDPARTYLICENSE \
    license=kernel/drivers/net/wifi/rwn/THIRDPARTYLICENSE
license kernel/drivers/net/wifi/rwn/fw-rt2860/LICENSE \
    license=kernel/drivers/net/wifi/rwn/fw-rt2860/LICENSE
