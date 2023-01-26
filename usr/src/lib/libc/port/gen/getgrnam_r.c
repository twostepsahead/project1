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
 * Copyright 2009 Sun Microsystems, Inc.  All rights reserved.
 * Use is subject to license terms.
 * Copyright (c) 2016 by Delphix. All rights reserved.
 */

#include "lint.h"
#include <mtlib.h>
#include <sys/types.h>
#include <grp.h>
#include <memory.h>
#include <deflt.h>
#include <nsswitch.h>
#include <nss_dbdefs.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <synch.h>
#include <sys/param.h>
#include <sys/mman.h>
#include <errno.h>

extern int _getgroupsbymember(const char *, gid_t[], int, int);
int str2group(const char *, int, void *, char *, int);

static DEFINE_NSS_DB_ROOT(db_root);
static DEFINE_NSS_GETENT(context);

#define	USE_NETID_STR	"NETID_AUTHORITATIVE=TRUE"

void
_nss_initf_group(nss_db_params_t *p)
{
	p->name	= NSS_DBNAM_GROUP;
	p->default_config = NSS_DEFCONF_GROUP;
}

#include <getxby_door.h>
#include <sys/door.h>

struct group *
_uncached_getgrnam_r(const char *name, struct group *result, char *buffer,
    int buflen);

struct group *
_uncached_getgrgid_r(gid_t gid, struct group *result, char *buffer, int buflen);

#pragma weak __posix_getgrnam_r = getgrnam_r
#pragma weak __posix_getgrgid_r = getgrgid_r

int
getgrnam_r(const char *name, struct group *grp, char *buffer, size_t buflen,
    struct group **result)
{
	nss_XbyY_args_t arg;

	if (!name) {
		errno = ERANGE;
		return errno;
	}
	NSS_XbyY_INIT(&arg, grp, buffer, buflen, str2group);
	arg.key.name = name;
	(void) nss_search(&db_root, _nss_initf_group, NSS_DBOP_GROUP_BYNAME,
	    &arg);
	*result = NSS_XbyY_FINI(&arg);
	if (!*result)
		return errno;
	return 0;
}

int
getgrgid_r(gid_t gid, struct group *grp, char *buffer, size_t buflen,
    struct group **result)
{
	nss_XbyY_args_t arg;

	NSS_XbyY_INIT(&arg, grp, buffer, buflen, str2group);
	arg.key.gid = gid;
	(void) nss_search(&db_root, _nss_initf_group, NSS_DBOP_GROUP_BYGID,
	    &arg);
	*result = NSS_XbyY_FINI(&arg);
	if (!*result)
		return errno;
	return 0;
}

struct group *
_uncached_getgrgid_r(gid_t gid, struct group *result, char *buffer,
    int buflen)
{
	nss_XbyY_args_t arg;

	NSS_XbyY_INIT(&arg, result, buffer, buflen, str2group);
	arg.key.gid = gid;
	(void) nss_search(&db_root, _nss_initf_group,
	    NSS_DBOP_GROUP_BYGID, &arg);
	return ((struct group *)NSS_XbyY_FINI(&arg));
}

struct group *
_uncached_getgrnam_r(const char *name, struct group *result, char *buffer,
	int buflen)
{
	nss_XbyY_args_t arg;

	NSS_XbyY_INIT(&arg, result, buffer, buflen, str2group);
	arg.key.name = name;
	(void) nss_search(&db_root, _nss_initf_group,
	    NSS_DBOP_GROUP_BYNAME, &arg);
	return ((struct group *)NSS_XbyY_FINI(&arg));
}

/*
 * _getgroupsbymember(uname, gid_array, maxgids, numgids):
 *	Private interface for initgroups().  It returns the group ids of
 *	groups of which the specified user is a member.
 *
 * Arguments:
 *   username	Username of the putative member
 *   gid_array	Space in which to return the gids.  The first [numgids]
 *		elements are assumed to already contain valid gids.
 *   maxgids	Maximum number of elements in gid_array.
 *   numgids	Number of elements (normally 0 or 1) that already contain
 *		valid gids.
 * Return value:
 *   number of valid gids in gid_array (may be zero)
 *	or
 *   -1 (and errno set appropriately) on errors (none currently defined)
 *
 * NSS2 Consistency enhancements:
 *   The "files normal" format between an application and nscd for the
 *   NSS_DBOP_GROUP_BYMEMBER nss_search operation is defined to be a
 *   processed array of numgids [up to maxgids] gid_t values.  gid_t
 *   values in the array are unique.
 */

extern nss_status_t process_cstr(const char *, int, struct nss_groupsbymem *);

int
_getgroupsbymember(const char *username, gid_t gid_array[],
    int maxgids, int numgids)
{
	struct nss_groupsbymem	arg;
	void	*defp;

	arg.username	= username;
	arg.gid_array	= gid_array;
	arg.maxgids	= maxgids;
	arg.numgids	= numgids;
	/*
	 * In backwards compatibility mode, use the old str2group &
	 * process_cstr interfaces.  Ditto within nscd processing.
	 */
	arg.str2ent	= str2group;
	arg.process_cstr = process_cstr;

	/*
	 * The old value being provided here was 0, ie do the quick
	 * way.  Given that this was never actually used under NIS
	 * and had the wrong (now corrected) meaning for NIS+ we need
	 * to change the default to be 1 (TRUE) as we now need the
	 * admin to decided to use netid, setting NETID_AUTHORITATIVE
	 * in /etc/default/nss to TRUE gets us a value of 0 for
	 * force_slow_way - don't you just love double negatives ;-)
	 *
	 * We need to do this to preserve the behaviour seen when the
	 * admin makes no changes.
	 */
	arg.force_slow_way = 1;

	if ((defp = defopen_r(__NSW_DEFAULT_FILE)) != NULL) {
		if (defread_r(USE_NETID_STR, defp) != NULL)
			arg.force_slow_way = 0;
		defclose_r(defp);
	}

	(void) nss_search(&db_root, _nss_initf_group,
	    NSS_DBOP_GROUP_BYMEMBER, &arg);

	return (arg.numgids);
}


static char *
gettok(char **nextpp, char sep)
{
	char	*p = *nextpp;
	char	*q = p;
	char	c;

	if (p == 0)
		return (0);

	while ((c = *q) != '\0' && c != sep)
		q++;

	if (c == '\0')
		*nextpp = 0;
	else {
		*q++ = '\0';
		*nextpp = q;
	}
	return (p);
}

/*
 * Return values: 0 = success, 1 = parse error, 2 = erange ...
 * The structure pointer passed in is a structure in the caller's space
 * wherein the field pointers would be set to areas in the buffer if
 * need be. instring and buffer should be separate areas.
 */
int
str2group(const char *instr, int lenstr, void *ent, char *buffer, int buflen)
{
	struct group		*group	= (struct group *)ent;
	char			*p, *next;
	char			**memlist, **limit;
	ulong_t			tmp;

	if (lenstr + 1 > buflen)
		return (NSS_STR_PARSE_ERANGE);

	/*
	 * We copy the input string into the output buffer and
	 * operate on it in place.
	 */
	if (instr != buffer) {
		/* Overlapping buffer copies are OK */
		(void) memmove(buffer, instr, lenstr);
		buffer[lenstr] = '\0';
	}

	/* quick exit do not entry fill if not needed */
	if (ent == NULL)
		return (NSS_STR_PARSE_SUCCESS);

	next = buffer;

	/*
	 * Parsers for passwd and group have always been pretty rigid;
	 * we wouldn't want to buck a Unix tradition
	 */

	group->gr_name = p = gettok(&next, ':');
	if (*p == '\0') {
		/* Empty group-name;  not allowed */
		return (NSS_STR_PARSE_PARSE);
	}

	/* Always return at least an empty gr_mem list */
	memlist	= (char **)ROUND_UP(buffer + lenstr + 1, sizeof (char *));
	limit	= (char **)ROUND_DOWN(buffer + buflen, sizeof (char *));
	*memlist = 0;
	group->gr_mem = memlist;

	group->gr_passwd = p = gettok(&next, ':');
	if (p == 0)
		return (NSS_STR_PARSE_PARSE);

	p = next;					/* gid */
	if (p == 0 || *p == '\0')
		return (NSS_STR_PARSE_PARSE);

	errno = 0;
	tmp = strtoul(p, &next, 10);
	if (next == p || errno != 0) {
		/* gid field should be nonempty */
		/* also check errno from strtoul */
		return (NSS_STR_PARSE_PARSE);
	}
	if (tmp >= UINT32_MAX)
		group->gr_gid = GID_NOBODY;
	else
		group->gr_gid = (gid_t)tmp;

	if (*next++ != ':')
		return (NSS_STR_PARSE_PARSE);

	/* === Could check and complain if there are any extra colons */
	while (memlist < limit) {
		p = gettok(&next, ',');
		if (p == 0 || *p == '\0') {
			*memlist = 0;
			/* Successfully parsed and stored */
			return (NSS_STR_PARSE_SUCCESS);
		}
		*memlist++ = p;
	}
	/* Out of space */
	return (NSS_STR_PARSE_ERANGE);
}

nss_status_t
process_cstr(const char *instr, int instr_len, struct nss_groupsbymem *gbm)
{
	/*
	 * It's possible to do a much less inefficient version of this by
	 * selectively duplicating code from str2group().  For now,
	 * however, we'll take the easy way out and implement this on
	 * top of str2group().
	 */

	const char		*username = gbm->username;
	nss_XbyY_buf_t		*buf;
	struct group		*grp;
	char			**memp;
	char			*mem;
	int	parsestat;

	buf = _nss_XbyY_buf_alloc(sizeof (struct group), NSS_BUFLEN_GROUP);
	if (buf == 0)
		return (NSS_UNAVAIL);

	grp = (struct group *)buf->result;

	parsestat = (*gbm->str2ent)(instr, instr_len,
	    grp, buf->buffer, buf->buflen);

	if (parsestat != NSS_STR_PARSE_SUCCESS) {
		_nss_XbyY_buf_free(buf);
		return (NSS_NOTFOUND);	/* === ? */
	}

	if (grp->gr_mem) {
		for (memp = grp->gr_mem; (memp) && ((mem = *memp) != 0);
		    memp++) {
			if (strcmp(mem, username) == 0) {
				gid_t	gid 	= grp->gr_gid;
				gid_t	*gidp	= gbm->gid_array;
				int	numgids	= gbm->numgids;
				int	i;

				_nss_XbyY_buf_free(buf);

				for (i = 0; i < numgids && *gidp != gid; i++,
				    gidp++) {
					;
				}
				if (i >= numgids) {
					if (i >= gbm->maxgids) {
					/* Filled the array;  stop searching */
						return (NSS_SUCCESS);
					}
					*gidp = gid;
					gbm->numgids = numgids + 1;
				}
				return (NSS_NOTFOUND);	/* Explained in   */
							/* <nss_dbdefs.h> */
			}
		}
	}
	_nss_XbyY_buf_free(buf);
	return (NSS_NOTFOUND);
}
