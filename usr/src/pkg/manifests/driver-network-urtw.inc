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
driver name=urtw clone_perms="urtw 0666 root sys" perms="* 0666 root sys" \
    alias=usb114b,150 \
    alias=usb1371,9401 \
    alias=usb50d,705e \
    alias=usb769,11f2 \
    alias=usb789,10c \
    alias=usb846,4260 \
    alias=usb846,6a00 \
    alias=usbbda,8187 \
    alias=usbbda,8189 \
    alias=usbbda,8197 \
    alias=usbbda,8198 \
    alias=usbdf6,28
file path=kernel/drv/urtw group=sys
file path=usr/share/man/man7d/urtw.7d
license cr_Sun license=cr_Sun
license kernel/drivers/net/wifi/urtw/THIRDPARTYLICENSE \
    license=kernel/drivers/net/wifi/urtw/THIRDPARTYLICENSE
