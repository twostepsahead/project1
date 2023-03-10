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

dir path=lib
dir path=lib/svc
dir path=lib/svc/manifest group=sys
dir path=lib/svc/manifest/network group=sys
dir path=lib/svc/manifest/network/smb group=sys
dir path=lib/svc/method
dir path=usr group=sys
dir path=usr/bin
dir path=usr/kernel group=sys
dir path=usr/kernel/drv group=sys
dir path=usr/kernel/fs group=sys
dir path=usr/kernel/kmdb group=sys
dir path=usr/lib
dir path=usr/lib/$(ARCH32)
dir path=usr/lib/fs group=sys
dir path=usr/lib/fs/smbfs group=sys
dir path=usr/lib/fs/smbfs/$(ARCH32) group=sys
dir path=usr/lib/mdb group=sys
dir path=usr/lib/mdb/kvm group=sys
dir path=usr/lib/mdb/kvm/$(ARCH64) group=sys
dir path=usr/lib/security
dir path=usr/lib/security/$(ARCH64)
dir path=usr/lib/smbfs
dir path=usr/share/man
dir path=usr/share/man/man1
dir path=usr/share/man/man8
dir path=usr/share/man/man4
dir path=usr/share/man/man5
dir path=usr/share/man/man7fs
driver name=nsmb perms="* 0666 root sys"
file path=lib/svc/manifest/network/smb/client.xml group=sys mode=0444
file path=lib/svc/method/smb-client mode=0555
file path=usr/bin/smbutil mode=0555
file path=usr/kernel/drv/nsmb group=sys
file path=usr/kernel/drv/nsmb.conf group=sys
file path=usr/kernel/fs/smbfs group=sys mode=0755
file path=usr/kernel/kmdb/nsmb group=sys mode=0555
file path=usr/kernel/kmdb/smbfs group=sys mode=0555
file path=usr/lib/$(ARCH32)/libsmbfs.so.1
file path=usr/lib/fs/smbfs/$(ARCH64)/libshare_smbfs.so.1
file path=usr/lib/fs/smbfs/dfshares mode=0555
file path=usr/lib/fs/smbfs/libshare_smbfs.so.1
file path=usr/lib/fs/smbfs/mount mode=4555
file path=usr/lib/fs/smbfs/share mode=0555
file path=usr/lib/fs/smbfs/umount mode=4555
file path=usr/lib/fs/smbfs/unshare mode=0555
file path=usr/lib/libmlrpc.so.2
file path=usr/lib/libsmbfs.so.1
file path=usr/lib/mdb/kvm/$(ARCH64)/nsmb.so mode=0555
file path=usr/lib/mdb/kvm/$(ARCH64)/smbfs.so mode=0555
file path=usr/lib/security/$(ARCH64)/pam_smbfs_login.so.1
file path=usr/lib/security/pam_smbfs_login.so.1
file path=usr/lib/smbfs/smbiod mode=0555
file path=usr/lib/smbfs/smbiod-svc mode=0555
file path=usr/share/man/man1/smbutil.1
file path=usr/share/man/man8/mount_smbfs.8
file path=usr/share/man/man8/smbiod.8
file path=usr/share/man/man4/nsmbrc.4
file path=usr/share/man/man5/pam_smbfs_login.5
file path=usr/share/man/man7fs/smbfs.7fs
license cr_Sun license=cr_Sun
license lic_CDDL license=lic_CDDL
license usr/src/lib/libsmbfs/smb/THIRDPARTYLICENSE.apple \
    license=usr/src/lib/libsmbfs/smb/THIRDPARTYLICENSE.apple
license usr/src/lib/libsmbfs/smb/THIRDPARTYLICENSE.boris_popov \
    license=usr/src/lib/libsmbfs/smb/THIRDPARTYLICENSE.boris_popov
license usr/src/lib/libsmbfs/smb/THIRDPARTYLICENSE.bsd4 \
    license=usr/src/lib/libsmbfs/smb/THIRDPARTYLICENSE.bsd4
license usr/src/lib/libsmbfs/smb/THIRDPARTYLICENSE.microsoft \
    license=usr/src/lib/libsmbfs/smb/THIRDPARTYLICENSE.microsoft
link path=usr/lib/security/$(ARCH32)/pam_smbfs_login.so \
    target=pam_smbfs_login.so.1
link path=usr/lib/security/pam_smbfs_login.so target=pam_smbfs_login.so.1
