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
dir path=usr/share/man
dir path=usr/share/man/man7d
driver name=nv_sata class=scsi-self-identifying perms="* 0644 root sys" \
    alias=pci10de,266 \
    alias=pci10de,267 \
    alias=pci10de,36 \
    alias=pci10de,37e \
    alias=pci10de,37f \
    alias=pci10de,3e \
    alias=pci10de,3f6 \
    alias=pci10de,3f7 \
    alias=pci10de,54 \
    alias=pci10de,55
file path=kernel/drv/nv_sata group=sys
file path=kernel/drv/nv_sata.conf group=sys
file path=usr/share/man/man7d/nv_sata.7d
license cr_Sun license=cr_Sun
license lic_CDDL license=lic_CDDL
