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
# Copyright (c) 2014 Racktop Systems.
# Copyright 2015, OmniTI Computer Consulting, Inc. All rights reserved.
#

<transform file path=usr/perl5/.*\.(pm|bs) -> default mode 0444>
<transform file path=usr/perl5/.*\.so -> default mode 0555>
dir path=usr group=sys
dir path=usr/perl5
dir path=usr/perl5/$(PERL_VERSION)
dir path=usr/perl5/$(PERL_VERSION)/lib
dir path=usr/perl5/$(PERL_VERSION)/lib/$(PERL_ARCH)
dir path=usr/perl5/$(PERL_VERSION)/lib/$(PERL_ARCH)/Sun
dir path=usr/perl5/$(PERL_VERSION)/lib/$(PERL_ARCH)/Sun/Solaris
dir path=usr/perl5/$(PERL_VERSION)/lib/$(PERL_ARCH)/auto
dir path=usr/perl5/$(PERL_VERSION)/lib/$(PERL_ARCH)/auto/Sun
dir path=usr/perl5/$(PERL_VERSION)/lib/$(PERL_ARCH)/auto/Sun/Solaris
dir path=usr/perl5/$(PERL_VERSION)/lib/$(PERL_ARCH)/auto/Sun/Solaris/Intrs
dir path=usr/perl5/$(PERL_VERSION)/lib/$(PERL_ARCH)/auto/Sun/Solaris/Kstat
dir path=usr/perl5/$(PERL_VERSION)/lib/$(PERL_ARCH)/auto/Sun/Solaris/Lgrp
dir path=usr/perl5/$(PERL_VERSION)/lib/$(PERL_ARCH)/auto/Sun/Solaris/Project
dir path=usr/perl5/$(PERL_VERSION)/lib/$(PERL_ARCH)/auto/Sun/Solaris/Task
dir path=usr/perl5/$(PERL_VERSION)/lib/$(PERL_ARCH)/auto/Sun/Solaris/Utils
$(ENABLE_PERL64)dir path=usr/perl5/$(PERL_VERSION)/lib/$(PERL_ARCH64)
$(ENABLE_PERL64)dir path=usr/perl5/$(PERL_VERSION)/lib/$(PERL_ARCH64)/Sun
$(ENABLE_PERL64)dir \
    path=usr/perl5/$(PERL_VERSION)/lib/$(PERL_ARCH64)/Sun/Solaris
$(ENABLE_PERL64)dir path=usr/perl5/$(PERL_VERSION)/lib/$(PERL_ARCH64)/auto
$(ENABLE_PERL64)dir path=usr/perl5/$(PERL_VERSION)/lib/$(PERL_ARCH64)/auto/Sun
$(ENABLE_PERL64)dir \
    path=usr/perl5/$(PERL_VERSION)/lib/$(PERL_ARCH64)/auto/Sun/Solaris
$(ENABLE_PERL64)dir \
    path=usr/perl5/$(PERL_VERSION)/lib/$(PERL_ARCH64)/auto/Sun/Solaris/Intrs
$(ENABLE_PERL64)dir \
    path=usr/perl5/$(PERL_VERSION)/lib/$(PERL_ARCH64)/auto/Sun/Solaris/Kstat
$(ENABLE_PERL64)dir \
    path=usr/perl5/$(PERL_VERSION)/lib/$(PERL_ARCH64)/auto/Sun/Solaris/Lgrp
$(ENABLE_PERL64)dir \
    path=usr/perl5/$(PERL_VERSION)/lib/$(PERL_ARCH64)/auto/Sun/Solaris/Project
$(ENABLE_PERL64)dir \
    path=usr/perl5/$(PERL_VERSION)/lib/$(PERL_ARCH64)/auto/Sun/Solaris/Task
$(ENABLE_PERL64)dir \
    path=usr/perl5/$(PERL_VERSION)/lib/$(PERL_ARCH64)/auto/Sun/Solaris/Utils
dir path=usr/perl5/$(PERL_VERSION)/lib/Sun
dir path=usr/perl5/$(PERL_VERSION)/lib/Sun/Solaris
dir path=usr/perl5/$(PERL_VERSION)/lib/Sun/Solaris/BSM
dir path=usr/share/man
dir path=usr/share/man/man3perl
file path=usr/perl5/$(PERL_VERSION)/lib/$(PERL_ARCH)/Sun/Solaris/Intrs.pm
file path=usr/perl5/$(PERL_VERSION)/lib/$(PERL_ARCH)/Sun/Solaris/Kstat.pm
file path=usr/perl5/$(PERL_VERSION)/lib/$(PERL_ARCH)/Sun/Solaris/Lgrp.pm
file path=usr/perl5/$(PERL_VERSION)/lib/$(PERL_ARCH)/Sun/Solaris/Project.pm
file path=usr/perl5/$(PERL_VERSION)/lib/$(PERL_ARCH)/Sun/Solaris/Task.pm
file path=usr/perl5/$(PERL_VERSION)/lib/$(PERL_ARCH)/Sun/Solaris/Utils.pm
file \
    path=usr/perl5/$(PERL_VERSION)/lib/$(PERL_ARCH)/auto/Sun/Solaris/Intrs/Intrs.so
file \
    path=usr/perl5/$(PERL_VERSION)/lib/$(PERL_ARCH)/auto/Sun/Solaris/Kstat/Kstat.so
file \
    path=usr/perl5/$(PERL_VERSION)/lib/$(PERL_ARCH)/auto/Sun/Solaris/Lgrp/Lgrp.so
file \
    path=usr/perl5/$(PERL_VERSION)/lib/$(PERL_ARCH)/auto/Sun/Solaris/Project/Project.so
file \
    path=usr/perl5/$(PERL_VERSION)/lib/$(PERL_ARCH)/auto/Sun/Solaris/Task/Task.so
file \
    path=usr/perl5/$(PERL_VERSION)/lib/$(PERL_ARCH)/auto/Sun/Solaris/Utils/Utils.so
$(ENABLE_PERL64)file \
    path=usr/perl5/$(PERL_VERSION)/lib/$(PERL_ARCH64)/Sun/Solaris/Intrs.pm
$(ENABLE_PERL64)file \
    path=usr/perl5/$(PERL_VERSION)/lib/$(PERL_ARCH64)/Sun/Solaris/Kstat.pm
$(ENABLE_PERL64)file \
    path=usr/perl5/$(PERL_VERSION)/lib/$(PERL_ARCH64)/Sun/Solaris/Lgrp.pm
$(ENABLE_PERL64)file \
    path=usr/perl5/$(PERL_VERSION)/lib/$(PERL_ARCH64)/Sun/Solaris/Project.pm
$(ENABLE_PERL64)file \
    path=usr/perl5/$(PERL_VERSION)/lib/$(PERL_ARCH64)/Sun/Solaris/Task.pm
$(ENABLE_PERL64)file \
    path=usr/perl5/$(PERL_VERSION)/lib/$(PERL_ARCH64)/Sun/Solaris/Utils.pm
$(ENABLE_PERL64)file \
    path=usr/perl5/$(PERL_VERSION)/lib/$(PERL_ARCH64)/auto/Sun/Solaris/Intrs/Intrs.so
$(ENABLE_PERL64)file \
    path=usr/perl5/$(PERL_VERSION)/lib/$(PERL_ARCH64)/auto/Sun/Solaris/Kstat/Kstat.so
$(ENABLE_PERL64)file \
    path=usr/perl5/$(PERL_VERSION)/lib/$(PERL_ARCH64)/auto/Sun/Solaris/Lgrp/Lgrp.so
$(ENABLE_PERL64)file \
    path=usr/perl5/$(PERL_VERSION)/lib/$(PERL_ARCH64)/auto/Sun/Solaris/Project/Project.so
$(ENABLE_PERL64)file \
    path=usr/perl5/$(PERL_VERSION)/lib/$(PERL_ARCH64)/auto/Sun/Solaris/Task/Task.so
$(ENABLE_PERL64)file \
    path=usr/perl5/$(PERL_VERSION)/lib/$(PERL_ARCH64)/auto/Sun/Solaris/Utils/Utils.so
file path=usr/perl5/$(PERL_VERSION)/lib/Sun/Solaris/BSM/_BSMparse.pm
file path=usr/perl5/$(PERL_VERSION)/lib/Sun/Solaris/Pg.pm
file path=usr/share/man/man3perl/Kstat.3perl
file path=usr/share/man/man3perl/Lgrp.3perl
file path=usr/share/man/man3perl/Project.3perl
file path=usr/share/man/man3perl/Task.3perl
license cr_Sun license=cr_Sun
license usr/src/cmd/perl/THIRDPARTYLICENSE \
    license=usr/src/cmd/perl/THIRDPARTYLICENSE
