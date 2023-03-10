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
dir path=etc group=sys
dir path=etc/sock2path.d group=sys
dir path=kernel group=sys
dir path=kernel/drv group=sys
driver name=rds perms="* 0644 root sys"
driver name=rdsib perms="* 0644 root sys"
file path=etc/sock2path.d/driver%2Fnetwork%2Frds group=sys
file path=kernel/drv/rds group=sys
file path=kernel/drv/rds.conf group=sys
file path=kernel/drv/rdsib group=sys
file path=kernel/drv/rdsib.conf group=sys
license usr/src/uts/common/io/ib/clients/rds/THIRDPARTYLICENSE \
    license=usr/src/uts/common/io/ib/clients/rds/THIRDPARTYLICENSE
