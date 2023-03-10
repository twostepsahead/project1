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
dir path=usr/share/man
dir path=usr/share/man/man7d
dir path=usr/share/man/man7i
driver name=audio perms="* 0666 root sys"
driver name=audio1575 alias=pci10b9,5455
driver name=audioens \
    alias=pci1274,1371 \
    alias=pci1274,5880
driver name=audiopci alias=pci1274,5000
driver name=audiots alias=pci10b9,5451
file path=kernel/drv/audio group=sys
file path=kernel/drv/audio.conf group=sys
file path=kernel/drv/audio1575 group=sys
file path=kernel/drv/audioens group=sys
file path=kernel/drv/audiopci group=sys
file path=kernel/drv/audiots group=sys
file path=kernel/misc/ac97 group=sys mode=0755
file path=usr/share/man/man7d/audio.7d
file path=usr/share/man/man7d/audio1575.7d
file path=usr/share/man/man7d/audioens.7d
file path=usr/share/man/man7d/audiopci.7d
file path=usr/share/man/man7d/audiots.7d
file path=usr/share/man/man7i/audio.7i
file path=usr/share/man/man7i/dsp.7i
file path=usr/share/man/man7i/mixer.7i
license cr_Sun license=cr_Sun
license lic_CDDL license=lic_CDDL
