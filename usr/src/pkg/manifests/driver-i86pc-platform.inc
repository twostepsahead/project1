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
dir path=kernel/strmod group=sys
dir path=usr/share/man
dir path=usr/share/man/man7d
driver name=asy alias=pci11c1,480 perms="* 0666 root sys" \
    perms="*,cu 0600 uucp uucp"
driver name=cmdk perms="* 0640 root sys"
driver name=ecpp alias=lp perms="* 0666 root sys"
driver name=fd perms="* 0666 root sys"
driver name=fdc
file path=kernel/drv/asy group=sys
file path=kernel/drv/asy.conf group=sys
file path=kernel/drv/cmdk group=sys
file path=kernel/drv/cmdk.conf group=sys
file path=kernel/drv/ecpp group=sys
file path=kernel/drv/ecpp.conf group=sys
file path=kernel/drv/fd group=sys
file path=kernel/drv/fd.conf group=sys
file path=kernel/drv/fdc group=sys
file path=kernel/drv/fdc.conf group=sys
file path=kernel/misc/dadk group=sys mode=0755
file path=kernel/misc/gda group=sys mode=0755
file path=kernel/misc/strategy group=sys mode=0755
file path=kernel/strmod/vuid2ps2 group=sys mode=0755
file path=kernel/strmod/vuid3ps2 group=sys mode=0755
file path=kernel/strmod/vuidm3p group=sys mode=0755
file path=kernel/strmod/vuidm4p group=sys mode=0755
file path=kernel/strmod/vuidm5p group=sys mode=0755
file path=usr/share/man/man7d/asy.7d
file path=usr/share/man/man7d/cmdk.7d
file path=usr/share/man/man7d/ecpp.7d
file path=usr/share/man/man7d/fd.7d
license cr_Sun license=cr_Sun
license lic_CDDL license=lic_CDDL
