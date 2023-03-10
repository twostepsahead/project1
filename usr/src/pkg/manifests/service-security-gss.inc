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

dir path=etc group=sys
dir path=etc/gss group=sys
dir path=lib
dir path=lib/svc
dir path=lib/svc/manifest group=sys
dir path=lib/svc/manifest/network group=sys
dir path=lib/svc/manifest/network/rpc group=sys
dir path=usr/share/man
dir path=usr/share/man/man4
dir path=usr/share/man/man5
file path=etc/gss/gsscred.conf group=sys preserve=true
file path=etc/gss/mech group=sys preserve=true
file path=etc/gss/qop group=sys preserve=true
file path=lib/svc/manifest/network/rpc/gss.xml group=sys mode=0444
file path=usr/share/man/man4/gsscred.conf.4
file path=usr/share/man/man4/mech.4
file path=usr/share/man/man5/gss_auth_rules.5
license cr_Sun license=cr_Sun
license lic_CDDL license=lic_CDDL
