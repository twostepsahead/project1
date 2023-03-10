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
# Copyright 2008 Sun Microsystems, Inc.  All rights reserved.
# Use is subject to license terms.
#
# ident	"%Z%%M%	%I%	%E% SMI"
#

PROG =		hald-probe-acpi
OBJS =		probe-acpi.o logger.o acpi.o util_pm.o
SRCS =		probe-acpi.c

include ../../../Makefile.cmd
include ../../../Makefile.cmd.64
include ../../Makefile.hal

ROOTCMDDIR =	$(ROOTLIB_HAL)

LDLIBS +=	-lc -ldbus-1 -lhal -lglib-2.0

CPPFLAGS +=	$(HAL_DBUS_CPPFLAGS) $(HAL_CONFIG_CPPFLAGS) $(HAL_GLIB_CPPFLAGS)
CPPFLAGS +=	-I$(ROOT)/usr/include/hal -I../../utils -I../../hald
CSTD =	$(CSTD_GNU99)

.KEEP_STATE:

all: $(PROG)

logger.o: ../../hald/logger.c
	$(COMPILE.c) -o $@ ../../hald/logger.c
	$(POST_PROCESS_O)

acpi.o: ../../utils/acpi.c
	$(COMPILE.c) -o $@ ../../utils/acpi.c
	$(POST_PROCESS_O)

util_pm.o: ../../hald/util_pm.c
	$(COMPILE.c) -o $@ ../../hald/util_pm.c
	$(POST_PROCESS_O)

$(PROG): $(OBJS)
	$(LINK.c) -o $@ $(OBJS) $(LDLIBS)
	$(POST_PROCESS)

install: all $(ROOTCMD)

clean:
	$(RM) $(OBJS) $(PROG)

FRC:

include ../../../Makefile.targ
