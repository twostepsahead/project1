#
# CDDL HEADER START
#
# This file and its contents are supplied under the terms of the
# Common Development and Distribution License ("CDDL"), version 1.0.
# You may only use this file in accordance with the terms of version
# 1.0 of the CDDL.
#
# A full copy of the text of the CDDL should have accompanied this
# source.  A copy of the CDDL is also available via the Internet at
# http://www.illumos.org/license/CDDL.
#
# CDDL HEADER END
#

#
# Copyright 2014 Nexenta Systems, Inc. All rights reserved.
#

#
# The default for payload-bearing actions in this package is to appear in the
# global zone only.  See the include file for greater detail, as well as
# information about overriding the defaults.
#
dir path=kernel group=sys
dir path=kernel/drv group=sys
dir path=usr group=sys
dir path=usr/share
dir path=usr/share/man
dir path=usr/share/man/man7d
driver name=skd alias=pciex1b39,1 class=disk perms="* 0600 root sys"
file path=kernel/drv/skd group=sys
file path=kernel/drv/skd.conf group=sys
file path=usr/share/man/man7d/skd.7d
license lic_CDDL license=lic_CDDL
