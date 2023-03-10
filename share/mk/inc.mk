# $Id: inc.mk,v 1.7 2017/05/06 17:29:45 sjg Exp $
#
#	@(#) Copyright (c) 2008, Simon J. Gerraty
#
#	This file is provided in the hope that it will
#	be of use.  There is absolutely NO WARRANTY.
#	Permission to copy, redistribute or otherwise
#	use this file is hereby granted provided that 
#	the above copyright notice and this notice are
#	left intact. 
#      
#	Please send copies of changes and bug-fixes to:
#	sjg@crufty.net
#

.include <init.mk>

.if !empty(LIBOWN)
INC_INSTALL_OWN ?= -o ${LIBOWN} -g ${LIBGRP}
.endif
INCMODE ?= 444
INC_COPY ?= -C
INCSDIR ?= ${INCDIR}

realinstall:	incinstall
.if !target(incinstall)
incinstall:
.if !empty(INCS)
	${INSTALL} -d ${INC_INSTALL_OWN} -m 755 ${DESTDIR}${INCSDIR}
	cd ${.CURDIR} && ${INSTALL} ${INC_COPY} ${INC_INSTALL_OWN} -m ${INCMODE} ${INCS} ${DESTDIR}${INCSDIR}
.endif
.endif
