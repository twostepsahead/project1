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
# Copyright 2012 Nexenta Systems, Inc. All rights reserved.
#

dir path=lib variant.opensolaris.zone=global
dir path=lib/svc variant.opensolaris.zone=global
dir path=lib/svc/manifest group=sys variant.opensolaris.zone=global
dir path=lib/svc/manifest/system group=sys variant.opensolaris.zone=global
dir path=lib/svc/method variant.opensolaris.zone=global
dir path=usr group=sys
dir path=usr/bin
dir path=usr/include
dir path=usr/kernel group=sys
dir path=usr/kernel/drv group=sys
dir path=usr/lib
dir path=usr/lib/$(ARCH32)
dir path=usr/lib/rcm
dir path=usr/lib/rcm/modules
dir path=usr/sbin
dir path=usr/share
dir path=usr/share/lib
dir path=usr/share/lib/xml group=sys
dir path=usr/share/lib/xml/dtd group=sys
dir path=usr/share/man
dir path=usr/share/man/man8
dir path=usr/share/man/man3lib
dir path=usr/share/man/man3pool
dir path=var group=sys variant.opensolaris.zone=global
dir path=var/log/pool group=sys variant.opensolaris.zone=global
dir path=var/log/pool group=sys variant.opensolaris.zone=global
driver name=pool perms="pool 0666 root sys" perms="poolctl 0666 root sys"
file path=lib/svc/manifest/system/pools.xml group=sys mode=0444 \
    variant.opensolaris.zone=global
file path=lib/svc/method/svc-pools mode=0555 variant.opensolaris.zone=global
file path=usr/bin/poolstat mode=0555
file path=usr/include/pool.h
file path=usr/kernel/drv/pool group=sys
file path=usr/kernel/drv/pool.conf group=sys
file path=usr/lib/$(ARCH32)/libpool.so.1
file path=usr/lib/libpool.so.1
file path=usr/lib/rcm/modules/SUNW_pool_rcm.so mode=0555
file path=usr/sbin/pooladm mode=0555
file path=usr/sbin/poolbind mode=0555
file path=usr/sbin/poolcfg mode=0555
file path=usr/share/lib/xml/dtd/rm_pool.dtd.1
file path=usr/share/man/man8/pooladm.8
file path=usr/share/man/man8/poolbind.8
file path=usr/share/man/man8/poolcfg.8
file path=usr/share/man/man8/poolstat.8
file path=usr/share/man/man3lib/libpool.3lib
file path=usr/share/man/man3pool/pool_associate.3pool
file path=usr/share/man/man3pool/pool_component_info.3pool
file path=usr/share/man/man3pool/pool_component_to_elem.3pool
file path=usr/share/man/man3pool/pool_conf_alloc.3pool
file path=usr/share/man/man3pool/pool_dynamic_location.3pool
file path=usr/share/man/man3pool/pool_error.3pool
file path=usr/share/man/man3pool/pool_get_binding.3pool
file path=usr/share/man/man3pool/pool_get_pool.3pool
file path=usr/share/man/man3pool/pool_get_property.3pool
file path=usr/share/man/man3pool/pool_resource_create.3pool
file path=usr/share/man/man3pool/pool_value_alloc.3pool
file path=usr/share/man/man3pool/pool_walk_components.3pool
license cr_Sun license=cr_Sun
license lic_CDDL license=lic_CDDL
link path=usr/lib/$(ARCH32)/libpool.so target=libpool.so.1
link path=usr/lib/libpool.so target=./libpool.so.1
