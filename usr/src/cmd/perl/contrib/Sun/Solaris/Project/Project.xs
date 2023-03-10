/*
 * Copyright (c) 1999, 2007, Oracle and/or its affiliates. All rights reserved.
 * Copyright (c) 2014 Racktop Systems.
 */
/*
 * Project.xs contains XS wrappers for the project database maniplulation
 * functions as provided by libproject and described in getprojent(3EXACCT).
 */

/* Solaris includes. */
#include <zone.h>
#include <project.h>
#include <pool.h>
#include <sys/pool_impl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <rctl.h>
#include <stdio.h>

/* Perl includes. */
#include "EXTERN.h"
#include "perl.h"
#include "XSUB.h"

/*
 * Convert and save a struct project on the perl XS return stack.
 * In a void context it returns nothing, in a scalar context it returns just
 * the name of the project and in a list context it returns a 6-element list
 * consisting of (name, projid, comment, users, groups, attr), where users and
 * groups are references to arrays containing the appropriate lists.
 */
static int
pushret_project(const struct project *proj)
{
	char	**cp;
	AV	*ary;

	dSP;
	if (GIMME_V == G_SCALAR) {
		EXTEND(SP, 1);
		PUSHs(sv_2mortal(newSVpv(proj->pj_name, 0)));
		PUTBACK;
		return (1);
	} else if (GIMME_V == G_ARRAY) {
		EXTEND(SP, 6);
		PUSHs(sv_2mortal(newSVpv(proj->pj_name, 0)));
		PUSHs(sv_2mortal(newSViv(proj->pj_projid)));
		PUSHs(sv_2mortal(newSVpv(proj->pj_comment, 0)));
		ary = newAV();
		for (cp = proj->pj_users; *cp != NULL; cp++) {
			av_push(ary, newSVpv(*cp, 0));
		}
		PUSHs(sv_2mortal(newRV_noinc((SV *)ary)));
		ary = newAV();
		for (cp = proj->pj_groups; *cp != NULL; cp++) {
			av_push(ary, newSVpv(*cp, 0));
		}
		PUSHs(sv_2mortal(newRV_noinc((SV *)ary)));
		PUSHs(sv_2mortal(newSVpv(proj->pj_attr, 0)));
		PUTBACK;
		return (6);
	} else {
		return (0);
	}
}

static int
pwalk_cb(const projid_t project, void *walk_data)
{
	int *nitemsp;

	dSP;
	nitemsp = (int *) walk_data;
	EXTEND(SP, 1);
	PUSHs(sv_2mortal(newSViv(project)));
	(*nitemsp)++;
	PUTBACK;
	return (0);
}

/*
 * The XS code exported to perl is below here.  Note that the XS preprocessor
 * has its own commenting syntax, so all comments from this point on are in
 * that form.  Note also that the PUTBACK; lines are necessary to synchronise
 * the local and global views of the perl stack before calling pushret_project,
 * as the code generated by the perl XS compiler twiddles with the stack on
 * entry to an XSUB.
 */

MODULE = Sun::Solaris::Project PACKAGE = Sun::Solaris::Project
PROTOTYPES: ENABLE

 #
 # Define any constants that need to be exported.  By doing it this way we can
 # avoid the overhead of using the DynaLoader package, and in addition constants
 # defined using this mechanism are eligible for inlining by the perl
 # interpreter at compile time.
 #
BOOT:
	{
	HV *stash;
	char buf[128];
	stash = gv_stashpv("Sun::Solaris::Project", TRUE);
	newCONSTSUB(stash, "MAXPROJID", newSViv(MAXPROJID));
	newCONSTSUB(stash, "PROJNAME_MAX", newSViv(PROJNAME_MAX));
	newCONSTSUB(stash, "PROJF_PATH",
	    newSVpv(PROJF_PATH, sizeof (PROJF_PATH) - 1));
	newCONSTSUB(stash, "PROJECT_BUFSZ", newSViv(PROJECT_BUFSZ));
	newCONSTSUB(stash, "SETPROJ_ERR_TASK", newSViv(SETPROJ_ERR_TASK));
	newCONSTSUB(stash, "SETPROJ_ERR_POOL", newSViv(SETPROJ_ERR_POOL));
	newCONSTSUB(stash, "RCTL_GLOBAL_NOBASIC",
		newSViv(RCTL_GLOBAL_NOBASIC));
	newCONSTSUB(stash, "RCTL_GLOBAL_LOWERABLE",
		newSViv(RCTL_GLOBAL_LOWERABLE));
	newCONSTSUB(stash, "RCTL_GLOBAL_DENY_ALWAYS",
		newSViv(RCTL_GLOBAL_DENY_ALWAYS));
	newCONSTSUB(stash, "RCTL_GLOBAL_DENY_NEVER",
		newSViv(RCTL_GLOBAL_DENY_NEVER));
	newCONSTSUB(stash, "RCTL_GLOBAL_FILE_SIZE",
		newSViv(RCTL_GLOBAL_FILE_SIZE));
	newCONSTSUB(stash, "RCTL_GLOBAL_CPU_TIME",
		newSViv(RCTL_GLOBAL_CPU_TIME));
	newCONSTSUB(stash, "RCTL_GLOBAL_SIGNAL_NEVER",
		newSViv(RCTL_GLOBAL_SIGNAL_NEVER));
	newCONSTSUB(stash, "RCTL_GLOBAL_INFINITE",
		newSViv(RCTL_GLOBAL_INFINITE));
	newCONSTSUB(stash, "RCTL_GLOBAL_UNOBSERVABLE",
		newSViv(RCTL_GLOBAL_UNOBSERVABLE));
	newCONSTSUB(stash, "RCTL_GLOBAL_BYTES",
		newSViv(RCTL_GLOBAL_BYTES));
	newCONSTSUB(stash, "RCTL_GLOBAL_SECONDS",
		newSViv(RCTL_GLOBAL_SECONDS));
	newCONSTSUB(stash, "RCTL_GLOBAL_COUNT",
		newSViv(RCTL_GLOBAL_COUNT));
	sprintf(buf, "%llu", UINT64_MAX);
	newCONSTSUB(stash, "RCTL_MAX_VALUE",
		newSVpv(buf, strlen(buf)));
	}

projid_t
getprojid()

int
setproject(name, user_name, flags)
	const char	*name;
	const char	*user_name
	uint_t		flags

void
activeprojects()
PREINIT:
	int	nitems;
PPCODE:
	PUTBACK;
	nitems = 0;
	project_walk(&pwalk_cb, (void*)&nitems);
	XSRETURN(nitems);

void
getprojent()
PREINIT:
	struct project	proj, *projp;
	char		buf[PROJECT_BUFSZ];
PPCODE:
	PUTBACK;
	if ((projp = getprojent(&proj, buf, sizeof (buf)))) {
		XSRETURN(pushret_project(projp));
	} else {
		XSRETURN_EMPTY;
	}

void
setprojent()

void
endprojent()

void
getprojbyname(name)
	char	*name
PREINIT:
	struct project	proj, *projp;
	char		buf[PROJECT_BUFSZ];
PPCODE:
	PUTBACK;
	if ((projp = getprojbyname(name, &proj, buf, sizeof (buf)))) {
		XSRETURN(pushret_project(projp));
	} else {
		XSRETURN_EMPTY;
	}

void
getprojbyid(id)
	projid_t	id
PREINIT:
	struct project	proj, *projp;
	char		buf[PROJECT_BUFSZ];
PPCODE:
	PUTBACK;
	if ((projp = getprojbyid(id, &proj, buf, sizeof (buf)))) {
		XSRETURN(pushret_project(projp));
	} else {
		XSRETURN_EMPTY;
	}

void
getdefaultproj(user)
	char	*user
PREINIT:
	struct project	proj, *projp;
	char		buf[PROJECT_BUFSZ];
PPCODE:
	PUTBACK;
	if ((projp = getdefaultproj(user, &proj, buf, sizeof (buf)))) {
		XSRETURN(pushret_project(projp));
	} else {
		XSRETURN_EMPTY;
	}

void
fgetprojent(fh)
	FILE	*fh
PREINIT:
	struct project	proj, *projp;
	char		buf[PROJECT_BUFSZ];
PPCODE:
	PUTBACK;
	if ((projp = fgetprojent(fh, &proj, buf, sizeof (buf)))) {
		XSRETURN(pushret_project(projp));
	} else {
		XSRETURN_EMPTY;
	}

bool
inproj(user, proj)
	char	*user
	char	*proj
PREINIT:
	char	buf[PROJECT_BUFSZ];
CODE:
	RETVAL = inproj(user, proj, buf, sizeof (buf));
OUTPUT:
	RETVAL


int
getprojidbyname(proj)
	char	*proj
PREINIT:
	int	id;
PPCODE:
	if ((id = getprojidbyname(proj)) == -1) {
		XSRETURN_UNDEF;
	} else {
		XSRETURN_IV(id);
	}


# rctl_get_info(name)
#
# For the given rctl name, returns the list
# ($max, $flags), where $max is the integer value
# of the system rctl, and $flags are the rctl's
# global flags, as returned by rctlblk_get_global_flags
#
# This function is private to Project.pm
void
rctl_get_info(name)
	char	*name
PREINIT:
	rctlblk_t *blk1 = NULL;
	rctlblk_t *blk2 = NULL;
	rctlblk_t *tmp = NULL;
	rctl_priv_t priv;
	rctl_qty_t value;
	int flags = 0;
	int ret;
	int err = 0;
	char string[24];	/* 24 will always hold a uint64_t */
PPCODE:
	Newc(0, blk1, rctlblk_size(), char, rctlblk_t);
	if (blk1 == NULL) {
		err = 1;
		goto out;
	}
	Newc(1, blk2, rctlblk_size(), char, rctlblk_t);
	if (blk2 == NULL) {
		err = 1;
		goto out;
	}
	ret = getrctl(name, NULL, blk1, RCTL_FIRST);
	if (ret != 0) {
		err = 1;
		goto out;
	}
	priv = rctlblk_get_privilege(blk1);
	while (priv != RCPRIV_SYSTEM) {
		tmp = blk2;
		blk2 = blk1;
		blk1 = tmp;
		ret = getrctl(name, blk2, blk1, RCTL_NEXT);
		if (ret != 0) {
			err = 1;
			goto out;
		}
		priv = rctlblk_get_privilege(blk1);
	}
	value = rctlblk_get_value(blk1);
	flags = rctlblk_get_global_flags(blk1);
	ret = sprintf(string, "%llu", value);
	if (ret <= 0) {
		err = 1;
	}
	out:
	if (blk1)
		Safefree(blk1);
	if (blk2)
		Safefree(blk2);
	if (err)
		XSRETURN(0);

	XPUSHs(sv_2mortal(newSVpv(string, 0)));
	XPUSHs(sv_2mortal(newSViv(flags)));
	XSRETURN(2);

#
# pool_exists(name)
#
# Returns 0 a pool with the given name exists on the current system.
# Returns 1 if pools are disabled or the pool does not exist
#
# Used internally by project.pm to validate the project.pool attribute
#
# This function is private to Project.pm
void
pool_exists(name)
	char	*name
PREINIT:
	pool_conf_t *conf;
	pool_t *pool;
	pool_status_t status;
	int fd;
PPCODE:

	/*
	 * Determine if pools are enabled using /dev/pool directly, as
	 * libpool may not be present.
	 */
	if (getzoneid() != GLOBAL_ZONEID) {
		XSRETURN_IV(1);
	}
	if ((fd = open("/dev/pool", O_RDONLY)) < 0) {
		XSRETURN_IV(1);
	}
	if (ioctl(fd, POOL_STATUSQ, &status) < 0) {
		(void) close(fd);
		XSRETURN_IV(1);
	}
	close(fd);
	if (status.ps_io_state != 1) {
		XSRETURN_IV(1);
	}

	/*
	 * If pools are enabled, assume libpool is present.
	 */
	conf = pool_conf_alloc();
	if (conf == NULL) {
		XSRETURN_IV(1);
	}
	if (pool_conf_open(conf, pool_dynamic_location(), PO_RDONLY)) {
		pool_conf_free(conf);
		XSRETURN_IV(1);
	}
	pool = pool_get_pool(conf, name);
	if (pool == NULL) {
		pool_conf_close(conf);
		pool_conf_free(conf);
		XSRETURN_IV(1);
	}
	pool_conf_close(conf);
	pool_conf_free(conf);
	XSRETURN_IV(0);

