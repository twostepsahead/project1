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

dir path=kernel group=sys
dir path=kernel/drv group=sys
dir path=usr group=sys
dir path=usr/lib
dir path=usr/lib/fm
dir path=usr/lib/fm/dict
dir path=usr/lib/fm/eft
dir path=usr/lib/locale
dir path=usr/lib/locale/C
dir path=usr/lib/locale/C/LC_MESSAGES
dir path=usr/share/man
dir path=usr/share/man/man7d
driver name=dca \
    alias=pci108e,5454 \
    alias=pci108e,5455 \
    alias=pci108e,5456 \
    alias=pci108e,5457 \
    alias=pci14e4,5820 \
    alias=pci14e4,5821 \
    alias=pci14e4,5822 \
    alias=pci14e4,5825
file path=kernel/drv/dca group=sys
file path=kernel/drv/dca.conf group=sys preserve=true
file path=usr/lib/fm/dict/SCA1000.dict mode=0444
file path=usr/lib/fm/dict/SCA500.dict mode=0444
file path=usr/lib/fm/eft/sca1000.eft mode=0444
file path=usr/lib/fm/eft/sca500.eft mode=0444
file path=usr/lib/locale/C/LC_MESSAGES/SCA1000.mo mode=0444
file path=usr/lib/locale/C/LC_MESSAGES/SCA500.mo mode=0444
file path=usr/share/man/man7d/dca.7d
license cr_Sun license=cr_Sun
license lic_CDDL license=lic_CDDL
