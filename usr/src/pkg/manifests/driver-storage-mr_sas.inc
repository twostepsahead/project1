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
# Copyright 2013 Nexenta Systems, Inc. All rights reserved.
# Copyright 2015 Garrett D'Amore <garrett@damore.org>
# Copyright 2017 Citrus IT Limited. All rights reserved.
#

#
# The default for payload-bearing actions in this package is to appear in the
# global zone only.  See the include file for greater detail, as well as
# information about overriding the defaults.
#
dir path=kernel group=sys
dir path=kernel/drv group=sys
dir path=usr/share/man
dir path=usr/share/man/man7d
driver name=mr_sas class=scsi-self-identifying \
    alias=pciex1000,52 \
    alias=pciex1000,53 \
    alias=pciex1000,5b \
    alias=pciex1000,5d \
    alias=pciex1000,5f \
    alias=pciex1000,71 \
    alias=pciex1000,73 \
    alias=pciex1000,78 \
    alias=pciex1000,79 \
    alias=pciex1000,ce \
    alias=pciex1000,cf
file path=kernel/drv/mr_sas group=sys
file path=kernel/drv/mr_sas.conf group=sys
file path=usr/share/man/man7d/mr_sas.7d
license cr_Sun license=cr_Sun
license kernel/drivers/scsi/mr_sas/THIRDPARTYLICENSE \
    license=kernel/drivers/scsi/mr_sas/THIRDPARTYLICENSE
