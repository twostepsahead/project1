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
driver name=audio810 \
    alias=pci1022,7445 \
    alias=pci1022,746d \
    alias=pci1039,7012 \
    alias=pci10de,1b1 \
    alias=pci10de,26b \
    alias=pci10de,3a \
    alias=pci10de,59 \
    alias=pci10de,6a \
    alias=pci10de,8a \
    alias=pci10de,da \
    alias=pci10de,ea \
    alias=pci8086,2415 \
    alias=pci8086,2425 \
    alias=pci8086,2445 \
    alias=pci8086,2485 \
    alias=pci8086,24c5 \
    alias=pci8086,24d5 \
    alias=pci8086,25a6 \
    alias=pci8086,266e \
    alias=pci8086,2698 \
    alias=pci8086,27de \
    alias=pci8086,7195
file path=kernel/drv/audio810 group=sys
file path=kernel/drv/audio810.conf group=sys preserve=true
file path=usr/share/man/man7d/audio810.7d
license cr_Sun license=cr_Sun
license lic_CDDL license=lic_CDDL
