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
# Copyright 2014 Nexenta Systems, Inc.  All rights reserved.
#

dir path=lib
dir path=lib/svc
dir path=lib/svc/manifest group=sys
dir path=lib/svc/manifest/network group=sys
dir path=lib/svc/manifest/network/smb group=sys
dir path=lib/svc/method
dir path=usr group=sys
dir path=usr/kernel group=sys
dir path=usr/kernel/drv group=sys
dir path=usr/kernel/kmdb group=sys
dir path=usr/lib
dir path=usr/lib/fs group=sys
dir path=usr/lib/fs/smb group=sys
dir path=usr/lib/fs/smb/$(ARCH32) group=sys
dir path=usr/lib/mdb group=sys
dir path=usr/lib/mdb/kvm group=sys
dir path=usr/lib/mdb/kvm/$(ARCH64) group=sys
dir path=usr/lib/reparse
dir path=usr/lib/security
dir path=usr/lib/smbsrv
dir path=usr/lib/smbsrv/dtrace
dir path=usr/sbin
dir path=usr/share/man
dir path=usr/share/man/man8
dir path=usr/share/man/man4
dir path=usr/share/man/man5
dir path=var group=sys
dir path=var/smb group=sys
dir path=var/smb/cvol group=sys
dir path=var/smb/cvol/windows group=sys
dir path=var/smb/cvol/windows/system32 group=sys
dir path=var/smb/cvol/windows/system32/vss group=sys
dir path=var/svc group=sys
driver name=smbsrv perms="* 0640 root sys"
file path=lib/svc/manifest/network/smb/server.xml group=sys mode=0444
file path=lib/svc/method/svc-smbd mode=0555
file path=usr/kernel/drv/smbsrv group=sys
file path=usr/kernel/drv/smbsrv.conf group=sys
file path=usr/kernel/kmdb/smbsrv group=sys mode=0555
file path=usr/lib/fs/smb/$(ARCH64)/libshare_smb.so.1
file path=usr/lib/fs/smb/libshare_smb.so.1
file path=usr/lib/mdb/kvm/$(ARCH64)/smbsrv.so group=sys mode=0555
file path=usr/lib/reparse/libreparse_smb.so.1
file path=usr/lib/security/pam_smb_passwd.so.1
file path=usr/lib/smbsrv/dtrace/smbd-all.d mode=0555
file path=usr/lib/smbsrv/dtrace/smbd-authsvc.d mode=0555
file path=usr/lib/smbsrv/dtrace/smbd-doorsvc.d mode=0555
file path=usr/lib/smbsrv/dtrace/smbd-pipesvc.d mode=0555
file path=usr/lib/smbsrv/dtrace/smbnode.d mode=0555
file path=usr/lib/smbsrv/dtrace/smbsrv.d mode=0555
file path=usr/lib/smbsrv/dtrace/smbvfs.d mode=0555
file path=usr/lib/smbsrv/libmlsvc.so.1
file path=usr/lib/smbsrv/libsmb.so.1
file path=usr/lib/smbsrv/libsmbns.so.1
file path=usr/lib/smbsrv/smbd mode=0555
file path=usr/sbin/smbadm mode=0555
file path=usr/sbin/smbstat mode=0555
file path=usr/share/man/man8/smbadm.8
file path=usr/share/man/man8/smbd.8
file path=usr/share/man/man8/smbstat.8
file path=usr/share/man/man4/smb.4
file path=usr/share/man/man4/smbautohome.4
file path=usr/share/man/man5/pam_smb_passwd.5
file path=var/smb/cvol/windows/system32/eventlog.dll mode=0755
file path=var/smb/smbpasswd group=sys mode=0400 preserve=true
license cr_Sun license=cr_Sun
license lic_CDDL license=lic_CDDL
link path=usr/lib/reparse/libreparse_smb.so target=libreparse_smb.so.1
link path=usr/lib/security/pam_smb_passwd.so target=pam_smb_passwd.so.1
