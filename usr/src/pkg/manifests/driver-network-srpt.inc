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
dir path=kernel/kmdb group=sys
dir path=lib variant.opensolaris.zone=global
dir path=lib/svc variant.opensolaris.zone=global
dir path=lib/svc/manifest group=sys variant.opensolaris.zone=global
dir path=lib/svc/manifest/system group=sys variant.opensolaris.zone=global
dir path=lib/svc/manifest/system/ibsrp group=sys \
    variant.opensolaris.zone=global
dir path=lib/svc/method variant.opensolaris.zone=global
dir path=usr group=sys
dir path=usr/include
dir path=usr/lib
dir path=usr/lib/$(ARCH32)
dir path=usr/lib/mdb group=sys
dir path=usr/lib/mdb/kvm group=sys
dir path=usr/lib/mdb/kvm/$(ARCH64) group=sys
dir path=usr/sbin
dir path=usr/share/man
dir path=usr/share/man/man7d
driver name=srpt perms="* 0644 root sys"
file path=kernel/drv/srpt group=sys
file path=kernel/drv/srpt.conf group=sys
file path=kernel/kmdb/srpt group=sys mode=0555
file path=lib/svc/manifest/system/ibsrp/target.xml group=sys mode=0444 \
    variant.opensolaris.zone=global
file path=lib/svc/method/svc-srpt mode=0555 variant.opensolaris.zone=global
file path=usr/include/libsrpt.h
file path=usr/lib/$(ARCH32)/libsrpt.so.1
file path=usr/lib/libsrpt.so.1
file path=usr/lib/mdb/kvm/$(ARCH64)/srpt.so group=sys mode=0555
file path=usr/sbin/srptadm mode=0555
file path=usr/share/man/man7d/srpt.7d
license cr_Sun license=cr_Sun
license lic_CDDL license=lic_CDDL
link path=usr/lib/$(ARCH32)/libsrpt.so target=libsrpt.so.1
link path=usr/lib/libsrpt.so target=libsrpt.so.1
