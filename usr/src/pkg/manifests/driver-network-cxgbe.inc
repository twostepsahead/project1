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
# Copyright (c) 2016 by Chelsio Communications, Inc. All rights reserved.
#

dir path=kernel group=sys
dir path=kernel/drv group=sys
dir path=usr group=sys
dir path=usr/sbin
driver name=cxgbe clone_perms="cxgbe 0666 root sys" \
    perms="* 0666 root sys"
driver name=t4nex clone_perms="t4nex 0666 root sys" \
    devlink=type=ddi_ctl:devctl;minor1=t4nex\tt4nex\M2 \
    perms="* 0666 root sys" \
    alias=pciex1425,4400 \
    alias=pciex1425,4401 \
    alias=pciex1425,4402 \
    alias=pciex1425,4403 \
    alias=pciex1425,4404 \
    alias=pciex1425,4405 \
    alias=pciex1425,4406 \
    alias=pciex1425,4407 \
    alias=pciex1425,4408 \
    alias=pciex1425,4409 \
    alias=pciex1425,440a \
    alias=pciex1425,440d \
    alias=pciex1425,440e \
    alias=pciex1425,5400 \
    alias=pciex1425,5401 \
    alias=pciex1425,5402 \
    alias=pciex1425,5403 \
    alias=pciex1425,5404 \
    alias=pciex1425,5405 \
    alias=pciex1425,5406 \
    alias=pciex1425,5407 \
    alias=pciex1425,5408 \
    alias=pciex1425,5409 \
    alias=pciex1425,540a \
    alias=pciex1425,540b \
    alias=pciex1425,540c \
    alias=pciex1425,540d \
    alias=pciex1425,540e \
    alias=pciex1425,540f \
    alias=pciex1425,5410 \
    alias=pciex1425,5411 \
    alias=pciex1425,5412 \
    alias=pciex1425,5413 \
    alias=pciex1425,5414 \
    alias=pciex1425,5415 \
    alias=pciex1425,5416 \
    alias=pciex1425,5417 \
    alias=pciex1425,5418 \
    alias=pciex1425,5480 \
    alias=pciex1425,5481 \
    alias=pciex1425,5482 \
    alias=pciex1425,5486 \
    alias=pciex1425,5487 \
    alias=pciex1425,5488 \
    alias=pciex1425,5489 \
    alias=pciex1425,5490 \
    alias=pciex1425,5491 \
    alias=pciex1425,5492 \
    alias=pciex1425,5493 \
    alias=pciex1425,5494 \
    alias=pciex1425,5495 \
    alias=pciex1425,5496 \
    alias=pciex1425,5497 \
    alias=pciex1425,5498 \
    alias=pciex1425,5499 \
    alias=pciex1425,549a \
    alias=pciex1425,549b \
    alias=pciex1425,549c \
    alias=pciex1425,549d \
    alias=pciex1425,549e \
    alias=pciex1425,549f \
    alias=pciex1425,54a0 \
    alias=pciex1425,54a1 \
    alias=pciex1425,6400 \
    alias=pciex1425,6401 \
    alias=pciex1425,6402 \
    alias=pciex1425,6403 \
    alias=pciex1425,6404 \
    alias=pciex1425,6405 \
    alias=pciex1425,6406 \
    alias=pciex1425,6407 \
    alias=pciex1425,6408 \
    alias=pciex1425,6409 \
    alias=pciex1425,640d \
    alias=pciex1425,6410 \
    alias=pciex1425,6411 \
    alias=pciex1425,6414 \
    alias=pciex1425,6415 \
    alias=pciex1425,6480 \
    alias=pciex1425,6481
file path=kernel/drv/cxgbe group=sys
file path=kernel/drv/t4nex group=sys
file path=usr/sbin/cxgbetool mode=0555
license cr_Sun license=cr_Sun
license lic_CDDL license=lic_CDDL
