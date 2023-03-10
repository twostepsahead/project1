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

dir path=etc group=sys
dir path=etc/krb5 group=sys
dir path=lib
dir path=lib/svc
dir path=lib/svc/manifest group=sys
dir path=lib/svc/manifest/network group=sys
dir path=lib/svc/manifest/network/security group=sys
dir path=usr group=sys
dir path=usr/bin
dir path=usr/lib
dir path=usr/lib/$(ARCH32)
dir path=usr/lib/$(ARCH32)/gss
dir path=usr/lib/gss
dir path=usr/lib/krb5
dir path=usr/lib/krb5/$(ARCH32)
dir path=usr/lib/krb5/plugins
dir path=usr/lib/krb5/plugins/preauth
dir path=usr/lib/security
dir path=usr/lib/security/$(ARCH32)
dir path=usr/share/man
dir path=usr/share/man/man1
dir path=usr/share/man/man8
dir path=usr/share/man/man3lib
dir path=usr/share/man/man5
dir path=var group=sys
dir path=var/krb5 group=sys
dir path=var/krb5/rcache group=sys mode=1777
dir path=var/krb5/rcache/root group=sys mode=0700
file path=etc/krb5/krb5.conf group=sys preserve=true
file path=etc/krb5/warn.conf group=sys preserve=true
file path=lib/svc/manifest/network/security/kadmin.xml group=sys mode=0444
file path=lib/svc/manifest/network/security/krb5kdc.xml group=sys mode=0444
file path=lib/svc/manifest/network/security/ktkt_warn.xml group=sys mode=0444
file path=usr/bin/kdestroy mode=0555
file path=usr/bin/kinit mode=0555
file path=usr/bin/klist mode=0555
file path=usr/bin/kpasswd mode=0555
file path=usr/bin/krb5-config mode=0555
file path=usr/bin/ktutil mode=0555
file path=usr/lib/$(ARCH32)/gss/mech_krb5.so.1
file path=usr/lib/$(ARCH32)/libkrb5.so.1
file path=usr/lib/gss/mech_krb5.so.1
file path=usr/lib/krb5/$(ARCH64)/libkadm5clnt.so.1
file path=usr/lib/krb5/ktkt_warnd mode=0555
file path=usr/lib/krb5/libkadm5clnt.so.1
file path=usr/lib/krb5/libss.so.1
file path=usr/lib/krb5/plugins/preauth/pkinit.so.1
file path=usr/lib/libkrb5.so.1
file path=usr/lib/security/$(ARCH64)/pam_krb5.so.1
file path=usr/lib/security/$(ARCH64)/pam_krb5_migrate.so.1
file path=usr/lib/security/pam_krb5.so.1
file path=usr/lib/security/pam_krb5_migrate.so.1
file path=usr/share/man/man1/kdestroy.1
file path=usr/share/man/man1/kinit.1
file path=usr/share/man/man1/klist.1
file path=usr/share/man/man1/kpasswd.1
file path=usr/share/man/man1/krb5-config.1
file path=usr/share/man/man1/ktutil.1
file path=usr/share/man/man8/ktkt_warnd.8
file path=usr/share/man/man3lib/libkrb5.3lib
file path=usr/share/man/man5/pam_krb5.5
file path=usr/share/man/man5/pam_krb5_migrate.5
license cr_Sun license=cr_Sun
license lic_CDDL license=lic_CDDL
license usr/src/lib/gss_mechs/mech_krb5/THIRDPARTYLICENSE \
    license=usr/src/lib/gss_mechs/mech_krb5/THIRDPARTYLICENSE
link path=usr/lib/$(ARCH32)/gss/mech_krb5.so target=./mech_krb5.so.1
link path=usr/lib/$(ARCH32)/libkrb5.so target=./libkrb5.so.1
link path=usr/lib/gss/mech_krb5.so target=./mech_krb5.so.1
link path=usr/lib/krb5/$(ARCH64)/libkadm5clnt.so target=./libkadm5clnt.so.1
link path=usr/lib/krb5/libkadm5clnt.so target=./libkadm5clnt.so.1
link path=usr/lib/krb5/libss.so target=libss.so.1
link path=usr/lib/krb5/plugins/preauth/pkinit.so target=pkinit.so.1
link path=usr/lib/libkrb5.so target=./libkrb5.so.1
link path=usr/lib/security/$(ARCH64)/pam_krb5.so target=./pam_krb5.so.1
link path=usr/lib/security/$(ARCH64)/pam_krb5_migrate.so \
    target=./pam_krb5_migrate.so.1
link path=usr/lib/security/pam_krb5.so target=./pam_krb5.so.1
link path=usr/lib/security/pam_krb5_migrate.so target=./pam_krb5_migrate.so.1
