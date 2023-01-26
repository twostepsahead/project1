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
# Copyright (c) 2014 Racktop Systems.
# Copyright 2015, OmniTI Computer Consulting, Inc. All rights reserved.
#

include $(SRC)/lib/Makefile.lib

# PERL_VERSION and PERL_ARCH used to be set here,
# but as they were also needed in usr/src/pkg/Makefile,
# the definition was moved to usr/src/Makefile.master

PERLDIR = $(ADJUNCT_PROTO)/usr/perl5/$(PERL_VERSION)
PERLLIBDIR = $(PERLDIR)/lib/$(PERL_ARCH)
PERLLIBDIR64 = $(PERLDIR)/lib/$(PERL_ARCH64)
PERLINCDIR = $(PERLLIBDIR)/CORE
PERLINCDIR64 = $(PERLLIBDIR64)/CORE

PERLMOD = $(MODULE).pm
PERLEXT = $(MACH)/$(MODULE).so
PERLEXT64 = $(MACH64)/$(MODULE).so

ROOTPERLDIR = $(ROOT)/usr/perl5/$(PERL_VERSION)
ROOTPERLLIBDIR = $(ROOTPERLDIR)/lib/$(PERL_ARCH)
ROOTPERLMODDIR = $(ROOTPERLLIBDIR)/Sun/Solaris
ROOTPERLEXTDIR = $(ROOTPERLLIBDIR)/auto/Sun/Solaris/$(MODULE)
ROOTPERLLIBDIR64 = $(ROOTPERLDIR)/lib/$(PERL_ARCH64)
ROOTPERLMODDIR64 = $(ROOTPERLLIBDIR64)/Sun/Solaris
ROOTPERLEXTDIR64 = $(ROOTPERLLIBDIR64)/auto/Sun/Solaris/$(MODULE)

ROOTPERLMOD = $(ROOTPERLMODDIR)/$(MODULE).pm
ROOTPERLEXT = $(ROOTPERLEXTDIR)/$(MODULE).so
ROOTPERLMOD64 = $(ROOTPERLMODDIR64)/$(MODULE).pm
ROOTPERLEXT64 = $(ROOTPERLEXTDIR64)/$(MODULE).so

XSUBPP = $(PERL) $(PERLDIR)/lib/ExtUtils/xsubpp \
	-typemap $(PERLDIR)/lib/ExtUtils/typemap

CSTD = $(CSTD_GNU99)
