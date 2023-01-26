/*
 * CDDL HEADER START
 *
 * The contents of this file are subject to the terms of the
 * Common Development and Distribution License (the "License").
 * You may not use this file except in compliance with the License.
 *
 * You can obtain a copy of the license at usr/src/OPENSOLARIS.LICENSE
 * or http://www.opensolaris.org/os/licensing.
 * See the License for the specific language governing permissions
 * and limitations under the License.
 *
 * When distributing Covered Code, include this CDDL HEADER in each
 * file and include the License file at usr/src/OPENSOLARIS.LICENSE.
 * If applicable, add the following below this CDDL HEADER, with the
 * fields enclosed by brackets "[]" replaced with your own identifying
 * information: Portions Copyright [yyyy] [name of copyright owner]
 *
 * CDDL HEADER END
 */
/*
 * Copyright 2008 Sun Microsystems, Inc.  All rights reserved.
 * Use is subject to license terms.
 */

#include <syslog.h>
#include <errno.h>
#include <unistd.h>
#include <stropts.h>

#include "mp_utils.h"
MP_STATUS
MP_GetAssociatedTPGOidList(MP_OID oid, MP_OID_LIST **ppList)
{
	MP_STATUS mpStatus = MP_STATUS_SUCCESS;


	log(LOG_INFO, "MP_GetAssociatedTPGOidList()", " - enter");


	mpStatus = getAssociatedTPGOidList(oid, ppList);


	log(LOG_INFO, "MP_GetAssociatedTPGOidList()", " - exit");

	return (mpStatus);
}


MP_STATUS
getAssociatedTPGOidList(MP_OID oid, MP_OID_LIST **ppList)
{
	mp_iocdata_t mp_ioctl;

	uint64_t *objList = NULL;

	int numOBJ = 0;
	int i = 0;
	int ioctlStatus = 0;

	MP_STATUS mpStatus = MP_STATUS_SUCCESS;


	log(LOG_INFO, "getAssociatedTPGOidList()", " - enter");


	log(LOG_INFO, "getAssociatedTPGOidList()",
		"oid.objectSequenceNumber = %llx",
		oid.objectSequenceNumber);

	if (g_scsi_vhci_fd < 0) {
		log(LOG_INFO, "getAssociatedTPGOidList()",
		    "invalid driver file handle");
		log(LOG_INFO, "getAssociatedTPGOidList()", " - error exit");
		return (MP_STATUS_FAILED);
	}

	objList = (uint64_t *)calloc(1, DEFAULT_BUFFER_SIZE_TPG);
	if (NULL == objList) {
		log(LOG_INFO, "getAssociatedTPGOidList()",
			"no memory for objList(1)");
		log(LOG_INFO, "getAssociatedTPGOidList()",
			" - error exit");
		return (MP_STATUS_INSUFFICIENT_MEMORY);
	}

	(void) memset(&mp_ioctl, 0, sizeof (mp_iocdata_t));

	mp_ioctl.mp_cmd  = MP_GET_TPG_LIST;
	mp_ioctl.mp_ibuf = (caddr_t)&oid.objectSequenceNumber;
	mp_ioctl.mp_ilen = sizeof (oid.objectSequenceNumber);
	mp_ioctl.mp_obuf = (caddr_t)objList;
	mp_ioctl.mp_olen = DEFAULT_BUFFER_SIZE_TPG;
	mp_ioctl.mp_xfer = MP_XFER_READ;

	log(LOG_INFO, "getAssociatedTPGOidList()",
		"mp_ioctl.mp_cmd (MP_GET_TPG_LIST) : %d",
		mp_ioctl.mp_cmd);
	log(LOG_INFO, "getAssociatedTPGOidList()",
		"mp_ioctl.mp_obuf: %x", mp_ioctl.mp_obuf);
	log(LOG_INFO, "getAssociatedTPGOidList()",
		"mp_ioctl.mp_olen: %d", mp_ioctl.mp_olen);
	log(LOG_INFO, "getAssociatedTPGOidList()",
		"mp_ioctl.mp_xfer: %d (MP_XFER_READ)",
		mp_ioctl.mp_xfer);

	ioctlStatus = ioctl(g_scsi_vhci_fd, MP_CMD, &mp_ioctl);
	log(LOG_INFO, "getAssociatedTPGOidList()",
		"ioctl call returned ioctlStatus: %d",
		ioctlStatus);

	if (ioctlStatus < 0) {
		ioctlStatus = errno;
	}

	if ((ioctlStatus != 0) && (MP_MORE_DATA != mp_ioctl.mp_errno)) {

		log(LOG_INFO, "getAssociatedTPGOidList()",
		    "IOCTL call failed.  IOCTL error is: %d",
			ioctlStatus);
		log(LOG_INFO, "getAssociatedTPGOidList()",
		    "IOCTL call failed.  IOCTL error is: %s",
			strerror(ioctlStatus));
		log(LOG_INFO, "getAssociatedTPGOidList()",
		    "IOCTL call failed.  mp_ioctl.mp_errno: %x",
			mp_ioctl.mp_errno);


		free(objList);

		if (ENOTSUP == ioctlStatus) {
			mpStatus = MP_STATUS_UNSUPPORTED;
		} else if (0 == mp_ioctl.mp_errno) {
			mpStatus = MP_STATUS_FAILED;
		} else {
			mpStatus = getStatus4ErrorCode(mp_ioctl.mp_errno);
		}

		log(LOG_INFO, "getAssociatedTPGOidList()",
			" - error exit, returning %d to caller.", mpStatus);

		return (mpStatus);
	}

	log(LOG_INFO, "getAssociatedTPGOidList()",
		" - mp_ioctl.mp_alen : %d",
		mp_ioctl.mp_alen);
	log(LOG_INFO, "getAssociatedTPGOidList()",
		" - sizeof (uint64_t): %d",
		sizeof (uint64_t));

	numOBJ = mp_ioctl.mp_alen / sizeof (uint64_t);
	log(LOG_INFO, "getAssociatedTPGOidList()",
	    "Length of list: %d", numOBJ);

	if (numOBJ < 1) {
		log(LOG_INFO, "getAssociatedTPGOidList()",
			"driver returned empty list.");

		free(objList);

		*ppList = createOidList(1);
		if (NULL == *ppList) {
			log(LOG_INFO,
				"getAssociatedTPGOidList()",
				"no memory for MP_OID_LIST");
			log(LOG_INFO,
				"getAssociatedTPGOidList()",
				" - error exit");
			return (MP_STATUS_INSUFFICIENT_MEMORY);
		}

		return (MP_STATUS_SUCCESS);
	}

	if (mp_ioctl.mp_alen > DEFAULT_BUFFER_SIZE_TPG) {

		log(LOG_INFO, "getAssociatedTPGOidList()",
			"buffer size too small, need : %d",
			mp_ioctl.mp_alen);

		free(objList);

		objList = (uint64_t *)calloc(1, numOBJ * sizeof (uint64_t));
		if (NULL == objList) {
			log(LOG_INFO, "getAssociatedTPGOidList()",
				"no memory for objList(2)");
			log(LOG_INFO, "getAssociatedTPGOidList()",
				" - error exit");
			return (MP_STATUS_INSUFFICIENT_MEMORY);
		}

		(void) memset(&mp_ioctl, 0, sizeof (mp_iocdata_t));

		mp_ioctl.mp_cmd  = MP_GET_TPG_LIST;
		mp_ioctl.mp_ibuf = (caddr_t)&oid.objectSequenceNumber;
		mp_ioctl.mp_ilen = sizeof (oid.objectSequenceNumber);
		mp_ioctl.mp_obuf = (caddr_t)objList;
		mp_ioctl.mp_olen = numOBJ * sizeof (uint64_t);
		mp_ioctl.mp_xfer = MP_XFER_READ;

		log(LOG_INFO, "getAssociatedTPGOidList()",
			"mp_ioctl.mp_cmd (MP_GET_TPG_LIST) : %d",
			mp_ioctl.mp_cmd);
		log(LOG_INFO, "getAssociatedTPGOidList()",
			"mp_ioctl.mp_obuf: %x", mp_ioctl.mp_obuf);
		log(LOG_INFO, "getAssociatedTPGOidList()",
			"mp_ioctl.mp_olen: %d", mp_ioctl.mp_olen);
		log(LOG_INFO, "getAssociatedTPGOidList()",
			"mp_ioctl.mp_xfer: %d (MP_XFER_READ)",
			mp_ioctl.mp_xfer);


		ioctlStatus = ioctl(g_scsi_vhci_fd, MP_CMD, &mp_ioctl);
		log(LOG_INFO, "getAssociatedTPGOidList()",
			"ioctl call returned ioctlStatus: %d",
			ioctlStatus);

		if (ioctlStatus < 0) {
			ioctlStatus = errno;
		}

		if (ioctlStatus != 0) {

			log(LOG_INFO, "getAssociatedTPGOidList()",
				"IOCTL call failed.  IOCTL error is: %d",
				ioctlStatus);
			log(LOG_INFO, "getAssociatedTPGOidList()",
				"IOCTL call failed.  IOCTL error is: %s",
				strerror(ioctlStatus));
			log(LOG_INFO, "getAssociatedTPGOidList()",
				"IOCTL call failed.  mp_ioctl.mp_errno: %x",
				mp_ioctl.mp_errno);


			free(objList);

			if (ENOTSUP == ioctlStatus) {
				mpStatus = MP_STATUS_UNSUPPORTED;
			} else if (0 == mp_ioctl.mp_errno) {
				mpStatus = MP_STATUS_FAILED;
			} else {
				mpStatus =
					getStatus4ErrorCode(mp_ioctl.mp_errno);
			}

			log(LOG_INFO, "getAssociatedTPGOidList()",
				" - error exit");

			return (mpStatus);
		}
	}


	*ppList = createOidList(numOBJ);
	if (NULL == *ppList) {
		log(LOG_INFO, "getAssociatedTPGOidList()",
			"no memory for *ppList");
		free(objList);
		log(LOG_INFO, "getAssociatedTPGOidList()",
			" - error exit");
		return (MP_STATUS_INSUFFICIENT_MEMORY);
	}

	(*ppList)->oidCount = numOBJ;

	log(LOG_INFO, "getAssociatedTPGOidList()",
		"(*ppList)->oidCount = %d",
		(*ppList)->oidCount);

	for (i = 0; i < numOBJ; i++) {
		(*ppList)->oids[i].objectType =
			MP_OBJECT_TYPE_TARGET_PORT_GROUP;
		(*ppList)->oids[i].ownerId = g_pluginOwnerID;
		(*ppList)->oids[i].objectSequenceNumber = objList[i];

		log(LOG_INFO, "getAssociatedTPGOidList()",
			"(*ppList)->oids[%d].objectType           = %d",
			i, (*ppList)->oids[i].objectType);
		log(LOG_INFO, "getAssociatedTPGOidList()",
			"(*ppList)->oids[%d].ownerId              = %d",
			i, (*ppList)->oids[i].ownerId);
		log(LOG_INFO, "getAssociatedTPGOidList()",
			"(*ppList)->oids[%d].objectSequenceNumber = %llx",
			i, (*ppList)->oids[i].objectSequenceNumber);
	}

	free(objList);


	log(LOG_INFO, "getAssociatedTPGOidList()",
		" - exit");

	return (MP_STATUS_SUCCESS);
}
