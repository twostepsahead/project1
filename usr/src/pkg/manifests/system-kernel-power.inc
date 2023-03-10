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
dir path=etc/default group=sys
dir path=lib
dir path=lib/svc
dir path=lib/svc/manifest group=sys
dir path=lib/svc/manifest/system group=sys
dir path=lib/svc/method
dir path=usr group=sys
dir path=usr/bin
dir path=usr/kernel group=sys
dir path=usr/kernel/drv group=sys
dir path=usr/lib
dir path=usr/lib/power group=sys
dir path=usr/sbin
dir path=usr/share/man
dir path=usr/share/man/man1
dir path=usr/share/man/man8
dir path=usr/share/man/man4
dir path=usr/share/man/man7d
driver name=pm perms="* 0666 root sys"
file path=etc/default/power group=sys
file path=etc/power.conf group=sys preserve=true
file path=lib/svc/manifest/system/power.xml group=sys mode=0444
file path=lib/svc/method/svc-power mode=0555
file path=usr/bin/sys-suspend mode=4555
file path=usr/kernel/drv/pm group=sys
file path=usr/kernel/drv/pm.conf group=sys
file path=usr/lib/power/powerd mode=0555
file path=usr/sbin/pmconfig mode=4555
file path=usr/share/man/man1/sys-suspend.1
file path=usr/share/man/man8/pmconfig.8
file path=usr/share/man/man8/powerd.8
file path=usr/share/man/man4/power.conf.4
file path=usr/share/man/man7d/pm.7d
license cr_Sun license=cr_Sun
license lic_CDDL license=lic_CDDL
