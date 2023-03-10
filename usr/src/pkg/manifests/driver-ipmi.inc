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

dir path=usr/include
dir path=usr/include/sys
dir path=usr/include/sys
dir path=usr/kernel group=sys
dir path=usr/kernel/drv group=sys
dir path=usr/share
dir path=usr/share/man
dir path=usr/share/man/man7d
driver name=ipmi perms="* 0600 root sys"
file path=usr/include/sys/ipmi.h
file path=usr/kernel/drv/ipmi
file path=usr/kernel/drv/ipmi.conf
file path=usr/share/man/man7d/ipmi.7d
license lic_CDDL license=lic_CDDL
license usr/src/uts/intel/io/ipmi/THIRDPARTYLICENSE \
    license=usr/src/uts/intel/io/ipmi/THIRDPARTYLICENSE
