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
dir path=platform group=sys
dir path=platform/kernel group=sys
dir path=platform/kernel/drv group=sys
driver name=fipe \
    alias=pci8086,25f0 \
    alias=pci8086,360c
file path=platform/kernel/drv/fipe group=sys
file path=platform/kernel/drv/fipe.conf group=sys
license usr/src/uts/i86pc/io/fipe/THIRDPARTYLICENSE \
    license=usr/src/uts/i86pc/io/fipe/THIRDPARTYLICENSE