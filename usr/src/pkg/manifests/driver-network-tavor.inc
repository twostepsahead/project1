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
driver name=tavor perms="* 0666 root sys" \
    alias=pci15b3,5a44 \
    alias=pci15b3,5a45 \
    alias=pci15b3,6278 \
    alias=pci15b3,6279 \
    alias=pciex15b3,6278 \
    alias=pciex15b3,6279
file path=kernel/drv/tavor group=sys
file path=kernel/drv/tavor.conf group=sys
license cr_Sun license=cr_Sun
