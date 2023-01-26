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
dir path=kernel/kmdb group=sys
dir path=kernel/misc group=sys
dir path=kernel/misc/pmcs group=sys
dir path=usr group=sys
dir path=usr/lib
dir path=usr/lib/mdb group=sys
dir path=usr/lib/mdb/kvm group=sys
dir path=usr/lib/mdb/kvm/$(ARCH64) group=sys
driver name=pmcs alias=pciex11f8,8001 class=scsi-self-identifying \
    perms="* 0600 root sys"
file path=kernel/drv/pmcs group=sys
file path=kernel/drv/pmcs.conf group=sys preserve=true
file path=kernel/kmdb/pmcs group=sys mode=0555
file path=kernel/misc/pmcs/pmcs8001fw group=sys mode=0755
file path=usr/lib/mdb/kvm/$(ARCH64)/pmcs.so group=sys mode=0555
license cr_Sun license=cr_Sun
license lic_CDDL license=lic_CDDL
license usr/src/uts/common/io/scsi/adapters/pmcs/LICENSE \
    license=usr/src/uts/common/io/scsi/adapters/pmcs/LICENSE