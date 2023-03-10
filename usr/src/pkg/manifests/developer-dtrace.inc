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
# Copyright (c) 2012 by Delphix. All rights reserved.
#

dir path=usr group=sys
dir path=usr/include
dir path=usr/include/sys
dir path=usr/lib
dir path=usr/lib/$(ARCH32)
dir path=usr/lib/devfsadm group=sys
dir path=usr/lib/devfsadm/linkmod group=sys
dir path=usr/lib/dtrace
dir path=usr/lib/dtrace/64
dir path=usr/lib/mdb group=sys
dir path=usr/lib/mdb/kvm group=sys
dir path=usr/lib/mdb/kvm/$(ARCH64) group=sys
dir path=usr/lib/mdb/raw group=sys
dir path=usr/lib/mdb/raw/$(ARCH64) group=sys
dir path=usr/sbin
dir path=usr/sbin/$(ARCH32)
dir path=usr/sbin/$(ARCH32)
dir path=usr/share
dir path=usr/share/lib
dir path=usr/share/man/man8
dir path=usr/share/man/man3lib
file path=usr/include/dtrace.h
file path=usr/include/sys/dtrace.h
file path=usr/include/sys/dtrace_impl.h
file path=usr/include/sys/fasttrap.h
file path=usr/include/sys/fasttrap_impl.h
file path=usr/include/sys/fasttrap_isa.h
file path=usr/include/sys/lockstat.h
file path=usr/include/sys/sdt.h
file path=usr/lib/$(ARCH32)/libdtrace.so.1
file path=usr/lib/devfsadm/linkmod/SUNW_dtrace_link.so group=sys
file path=usr/lib/dtrace/64/drti.o mode=0444
file path=usr/lib/dtrace/64/libdtrace_forceload.so mode=0555
file path=usr/lib/dtrace/drti.o mode=0444
file path=usr/lib/dtrace/errno.d mode=0444
file path=usr/lib/dtrace/fc.d mode=0444
file path=usr/lib/dtrace/io.d mode=0444
file path=usr/lib/dtrace/ip.d mode=0444
file path=usr/lib/dtrace/iscsit.d mode=0444
file path=usr/lib/dtrace/libdtrace_forceload.so mode=0555
file path=usr/lib/dtrace/net.d mode=0444
file path=usr/lib/dtrace/nfs.d mode=0444
file path=usr/lib/dtrace/nfssrv.d mode=0444
file path=usr/lib/dtrace/procfs.d mode=0444
file path=usr/lib/dtrace/regs.d mode=0444
file path=usr/lib/dtrace/sched.d mode=0444
file path=usr/lib/dtrace/scsi.d mode=0444
file path=usr/lib/dtrace/signal.d mode=0444
file path=usr/lib/dtrace/srp.d mode=0444
file path=usr/lib/dtrace/sysevent.d mode=0444
file path=usr/lib/dtrace/tcp.d mode=0444
file path=usr/lib/dtrace/udp.d mode=0444
file path=usr/lib/dtrace/unistd.d mode=0444
file path=usr/lib/libdtrace.so.1
file path=usr/lib/mdb/kvm/$(ARCH64)/dtrace.so group=sys mode=0555
file path=usr/lib/mdb/raw/$(ARCH64)/dof.so group=sys mode=0555
file path=usr/lib/mdb/raw/dof.so group=sys mode=0555
file path=usr/sbin/$(ARCH32)/dtrace mode=0555
file path=usr/sbin/$(ARCH32)/intrstat mode=0555
file path=usr/sbin/$(ARCH32)/lockstat mode=0555
file path=usr/sbin/$(ARCH32)/plockstat mode=0555
file path=usr/sbin/$(ARCH64)/dtrace mode=0555
file path=usr/sbin/$(ARCH64)/intrstat mode=0555
file path=usr/sbin/$(ARCH64)/lockstat mode=0555
file path=usr/sbin/$(ARCH64)/plockstat mode=0555
file path=usr/share/man/man8/dtrace.8
file path=usr/share/man/man8/intrstat.8
file path=usr/share/man/man8/lockstat.8
file path=usr/share/man/man8/plockstat.8
file path=usr/share/man/man3lib/libdtrace.3lib
hardlink path=usr/sbin/dtrace target=../../usr/lib/isaexec
hardlink path=usr/sbin/intrstat target=../../usr/lib/isaexec
hardlink path=usr/sbin/lockstat target=../../usr/lib/isaexec
hardlink path=usr/sbin/plockstat target=../../usr/lib/isaexec
license cr_Sun license=cr_Sun
license lic_CDDL license=lic_CDDL
link path=usr/lib/$(ARCH32)/libdtrace.so target=libdtrace.so.1
link path=usr/lib/libdtrace.so target=libdtrace.so.1
