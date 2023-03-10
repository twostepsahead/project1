#
# This file and its contents are supplied under the terms of the
# Common Development and Distribution License ("CDDL"), version 1.0.
# You may only use this file in accordance with the terms of version
# 1.0 of the CDDL.
#
# A full copy of the text of the CDDL should have accompanied this
# source.  A copy of the CDDL is also available via the Internet at
# http://www.illumos.org/license/CDDL.
#

#
# Copyright 2016 Garrett D'Amore <garrett@damore.org>
#
# Note: The driver has untested support for the following
# PCI IDs.  Once the tests are complete (and assuming no problems
# are found), they can be added:
#
# pci1924,1903	- SFC9120 Virtual Function
# pci1924,1923	- SFC9140 Virtual Function
# pci1924,913	- Medford (PF, Uninitialized)
# pci1924,a03	- SFC9240
# pci1924,1a03	- SFC9240 Virtual Function

#
# The driver should work on SPARC if anyone wants to add the Makefile
# support and test it, but you need a PCIe based SPARC.  For now just
# don't deliver it until someone shows up to do this.
#
dir path=kernel group=sys
dir path=kernel/drv group=sys
dir path=usr/share/man
dir path=usr/share/man/man7d
driver name=sfxge clone_perms="sfxge 0666 root sys" perms="* 0666 root sys" \
    alias=pci1924,803 \
    alias=pci1924,810 \
    alias=pci1924,813 \
    alias=pci1924,901 \
    alias=pci1924,903 \
    alias=pci1924,923
file path=kernel/drv/sfxge group=sys
file path=usr/share/man/man7d/sfxge.7d
license kernel/drivers/net/sfxge/THIRDPARTYLICENSE \
    license=kernel/drivers/net/sfxge/THIRDPARTYLICENSE
