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
driver name=afe clone_perms="afe 0666 root sys" perms="* 0666 root sys" \
    alias=pci10b7,9300 \
    alias=pci1113,1216 \
    alias=pci1317,1985 \
    alias=pci1317,9511 \
    alias=pci1317,9513 \
    alias=pci1317,981 \
    alias=pci1317,985 \
    alias=pci13d1,ab02 \
    alias=pci13d1,ab03 \
    alias=pci13d1,ab08 \
    alias=pci1737,ab08
file path=kernel/drv/afe group=sys
file path=usr/share/man/man7d/afe.7d
license cr_Sun license=cr_Sun
license usr/src/uts/common/io/afe/THIRDPARTYLICENSE \
    license=usr/src/uts/common/io/afe/THIRDPARTYLICENSE
