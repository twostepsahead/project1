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

#
# The default for payload-bearing actions in this package is to appear in the
# global zone only.  See the include file for greater detail, as well as
# information about overriding the defaults.
#
dir path=kernel group=sys
dir path=kernel/drv group=sys
driver name=efe alias=pci10b8,5 clone_perms="efe 0666 root sys" \
    perms="* 0666 root sys"
file path=kernel/drv/efe group=sys
license usr/src/uts/common/io/efe/THIRDPARTYLICENSE \
    license=usr/src/uts/common/io/efe/THIRDPARTYLICENSE
