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
# Copyright (c) 2012, 2017 by Delphix. All rights reserved.
# Copyright (c) 2013 by Saso Kiselkov. All rights reserved.
#

#
# Skip manifest generation until build 134 can be required on the
# build systems, due to a Python flavor identification bug in pkgdepend.
#
dir path=etc group=sys
dir path=etc/fs group=sys
dir path=etc/fs/zfs group=sys
dir path=etc/sysevent group=sys
dir path=etc/sysevent/config group=sys
dir path=etc/zfs group=sys
dir path=kernel group=sys
dir path=kernel/drv group=sys
dir path=kernel/fs group=sys
dir path=kernel/kmdb group=sys
dir path=lib
dir path=lib/$(ARCH32)
dir path=sbin group=sys
dir path=usr group=sys
dir path=usr/lib
dir path=usr/lib/$(ARCH32)
dir path=usr/lib/devfsadm group=sys
dir path=usr/lib/devfsadm/linkmod group=sys
dir path=usr/lib/fs group=sys
dir path=usr/lib/fs/zfs group=sys
dir path=usr/lib/mdb group=sys
dir path=usr/lib/mdb/kvm group=sys
dir path=usr/lib/mdb/kvm/$(ARCH64) group=sys
dir path=usr/lib/mdb/proc group=sys
dir path=usr/lib/mdb/proc/$(ARCH32)
dir path=usr/lib/sysevent
dir path=usr/lib/sysevent/modules
dir path=usr/lib/zfs
dir path=usr/sbin
dir path=usr/sbin/$(ARCH32)
dir path=usr/sbin/$(ARCH32)
dir path=usr/share/man/man8
driver name=zfs perms="* 0600 root sys" perms="zfs 0666 root sys"
file \
    path=etc/sysevent/config/SUNW,EC_zfs,ESC_ZFS_bootfs_vdev_attach,sysevent.conf \
    group=sys
file path=kernel/drv/zfs group=sys
file path=kernel/drv/zfs.conf group=sys
file path=kernel/kmdb/zfs group=sys mode=0555
file path=lib/$(ARCH32)/libzfs.so.1
file path=lib/$(ARCH32)/libzfs_core.so.1
file path=lib/libzfs.so.1
file path=lib/libzfs_core.so.1
file path=sbin/zfs mode=0555
file path=sbin/zpool mode=0555
file path=usr/lib/$(ARCH32)/libzpool.so.1
file path=usr/lib/devfsadm/linkmod/SUNW_zfs_link.so group=sys
file path=usr/lib/fs/zfs/bootinstall mode=0555
file path=usr/lib/fs/zfs/fstyp.so.1 mode=0555
file path=usr/lib/libzpool.so.1
file path=usr/lib/mdb/kvm/$(ARCH64)/zfs.so group=sys mode=0555
file path=usr/lib/mdb/proc/$(ARCH64)/libzpool.so group=sys mode=0555
file path=usr/lib/mdb/proc/libzpool.so group=sys mode=0555
file path=usr/lib/sysevent/modules/zfs_mod.so group=sys
file path=usr/sbin/$(ARCH32)/zdb mode=0555
file path=usr/sbin/$(ARCH64)/zdb mode=0555
file path=usr/sbin/zstreamdump mode=0555
file path=usr/share/man/man8/zdb.8
file path=usr/share/man/man8/zfs-program.8
file path=usr/share/man/man8/zfs.8
file path=usr/share/man/man8/zpool.8
file path=usr/share/man/man8/zstreamdump.8
file path=usr/share/man/man5/zpool-features.5
hardlink path=kernel/fs/zfs target=../../kernel/drv/zfs
hardlink path=usr/lib/fs/zfs/fstyp target=../../../sbin/fstyp
hardlink path=usr/sbin/zdb target=../../usr/lib/isaexec
license cr_Sun license=cr_Sun
license kernel/fs/zfs/THIRDPARTYLICENSE.cityhash \
    license=kernel/fs/zfs/THIRDPARTYLICENSE.cityhash
license kernel/fs/zfs/THIRDPARTYLICENSE.lz4 \
    license=kernel/fs/zfs/THIRDPARTYLICENSE.lz4
license lic_CDDL license=lic_CDDL
link path=etc/fs/zfs/mount target=../../../sbin/zfs
link path=etc/fs/zfs/umount target=../../../sbin/zfs
link path=lib/$(ARCH32)/libzfs.so target=libzfs.so.1
link path=lib/$(ARCH32)/libzfs_core.so target=libzfs_core.so.1
link path=lib/libzfs.so target=libzfs.so.1
link path=lib/libzfs_core.so target=libzfs_core.so.1
link path=usr/lib/$(ARCH32)/libzfs.so \
    target=../../../lib/$(ARCH32)/libzfs.so.1
link path=usr/lib/$(ARCH32)/libzfs.so.1 \
    target=../../../lib/$(ARCH32)/libzfs.so.1
link path=usr/lib/$(ARCH32)/libzfs_core.so \
    target=../../../lib/$(ARCH32)/libzfs_core.so.1
link path=usr/lib/$(ARCH32)/libzfs_core.so.1 \
    target=../../../lib/$(ARCH32)/libzfs_core.so.1
link path=usr/lib/$(ARCH32)/libzpool.so target=libzpool.so.1
link path=usr/lib/fs/zfs/mount target=../../../../sbin/zfs
link path=usr/lib/fs/zfs/umount target=../../../../sbin/zfs
link path=usr/lib/libzfs.so target=../../lib/libzfs.so.1
link path=usr/lib/libzfs.so.1 target=../../lib/libzfs.so.1
link path=usr/lib/libzfs_core.so target=../../lib/libzfs_core.so.1
link path=usr/lib/libzfs_core.so.1 target=../../lib/libzfs_core.so.1
link path=usr/lib/libzpool.so target=libzpool.so.1
link path=usr/sbin/zfs target=../../sbin/zfs
link path=usr/sbin/zpool target=../../sbin/zpool
