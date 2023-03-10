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
# Copyright 2018 OmniOS Community Edition (OmniOSce) Association.
#

dir path=usr/lib/python$(PYTHON3_VERSION)
dir path=usr/lib/python$(PYTHON3_VERSION)/vendor-packages
file path=usr/lib/python$(PYTHON3_VERSION)/vendor-packages/libbe_py.so
license cr_Sun license=cr_Sun
license lic_CDDL license=lic_CDDL
depend fmri=runtime/python$(PYTHON3_PKGVERS) type=require
