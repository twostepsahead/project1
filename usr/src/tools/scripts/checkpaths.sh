#!/bin/ksh -p
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
# Copyright 2009 Sun Microsystems, Inc.  All rights reserved.
# Use is subject to license terms.
#
# Copyright (c) 2019, Joyent, Inc.

# Quis custodiet ipsos custodies?

if [ -z "$SRC" ]; then
	SRC=$SRCTOP/usr/src
fi

if [ -z "$SRCTOP" -o ! -d "$SRCTOP" -o ! -d "$SRC" ]; then
	echo "$0: must be run from within a workspace."
	exit 1
fi

cd $SRCTOP || exit 1

# Use -b to tell this script to ignore derived (built) objects.
if [ "$1" = "-b" ]; then
	b_flg=y
fi

# Not currently used; available for temporary workarounds.
args="-k NEVER_CHECK"

# We intentionally don't depend on $MACH here, and thus no $ROOT.  If
# a proto area exists, then we use it.  This allows this script to be
# run against gates (which should contain both SPARC and x86 proto
# areas), build workspaces (which should contain just one proto area),
# and unbuilt workspaces (which contain no proto areas).
if [ "$b_flg" = y ]; then
	rootlist=
elif [ $# -gt 0 ]; then
	rootlist=$*
else
	rootlist="$SRCTOP/proto/root_sparc $SRCTOP/proto/root_i386"
fi

for ROOT in $rootlist
do
	case "$ROOT" in
	*sparc|*sparc-nd)
		arch=sparc
		;;
	*i386|*i386-nd)
		arch=i386
		;;
	*)
		echo "$ROOT has unknown architecture." >&2
		exit 1
		;;
	esac
done

if [ -f $SRC/tools/opensolaris/license-list ]; then
	sed -e 's/$/.descrip/' < $SRC/tools/opensolaris/license-list | \
		validate_paths -n SRC/tools/opensolaris/license-list
fi

exit 0
