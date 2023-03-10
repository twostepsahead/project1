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
driver name=ntxn \
    alias=pci4040,1 \
    alias=pci4040,100 \
    alias=pci4040,2 \
    alias=pci4040,24 \
    alias=pci4040,25 \
    alias=pci4040,3 \
    alias=pci4040,4 \
    alias=pci4040,5
file path=kernel/drv/ntxn group=sys
file path=kernel/drv/ntxn.conf group=sys
file path=usr/share/man/man7d/ntxn.7d
license cr_Sun license=cr_Sun
license lic_CDDL license=lic_CDDL
license usr/src/uts/common/io/ntxn/THIRDPARTYLICENSE \
    license=usr/src/uts/common/io/ntxn/THIRDPARTYLICENSE
