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
 * Copyright (c) 2002, 2010, Oracle and/or its affiliates. All rights reserved.
 */

#include <nss_dbdefs.h>
#include <pwd.h>
#include <stdlib.h>
#include <string.h>
#include <syslog.h>
#include <unistd.h>
#include <auth_attr.h>
#include <deflt.h>
#include <priv.h>
#include <secdb.h>
#include <user_attr.h>
#include <sys/task.h>
#include <libintl.h>
#include <project.h>
#include <errno.h>
#include <alloca.h>

#include <security/pam_appl.h>
#include <security/pam_modules.h>
#include <security/pam_impl.h>

#define	PROJECT		"project="
#define	PROJSZ		(sizeof (PROJECT) - 1)

/*
 *	unix_cred - PAM auth modules must contain both pam_sm_authenticate
 *		and pam_sm_setcred.  Some other auth module is responsible
 *		for authentication (e.g., pam_unix_auth.so), this module
 *		only implements pam_sm_setcred so that the authentication
 *		can be separated without knowledge of the Solaris Unix style
 *		credential setting.
 *		Solaris Unix style credential setting includes
 *		setting the user's default and limit privileges.
 */

/*
 *	unix_cred - pam_sm_authenticate
 *
 *	Returns	PAM_IGNORE.
 */

/*ARGSUSED*/
int
pam_sm_authenticate(pam_handle_t *pamh, int flags, int argc, const char **argv)
{
	return (PAM_IGNORE);
}

/*
 * Set the privilege set.  The attributes are enumerated by _enum_attrs,
 * including the attribues user_attr, prof_attr and policy.conf
 */
static int
getset(char *str, priv_set_t **res)
{
	priv_set_t *tmp;
	char *badp;
	int len;

	if (str == NULL)
		return (0);

	len = strlen(str) + 1;
	badp = alloca(len);
	(void) memset(badp, '\0', len);
	do {
		const char *q, *endp;
		tmp = priv_str_to_set(str, ",", &endp);
		if (tmp == NULL) {
			if (endp == NULL)
				break;

			/* Now remove the bad privilege endp points to */
			q = strchr(endp, ',');
			if (q == NULL)
				q = endp + strlen(endp);

			if (*badp != '\0')
				(void) strlcat(badp, ",", len);
			/* Memset above guarantees NUL termination */
			/* LINTED */
			(void) strncat(badp, endp, q - endp);
			/* excise bad privilege; strtok ignores 2x sep */
			(void) memmove((void *)endp, q, strlen(q) + 1);
		}
	} while (tmp == NULL && *str != '\0');

	if (tmp == NULL) {
		syslog(LOG_AUTH|LOG_ERR,
		    "pam_setcred: can't parse privilege specification: %m\n");
		return (-1);
	} else if (*badp != '\0') {
		syslog(LOG_AUTH|LOG_DEBUG,
		    "pam_setcred: unrecognized privilege(s): %s\n", badp);
	}
	*res = tmp;
	return (0);
}

typedef struct deflim {
	char *def;
	char *lim;
} deflim_t;

/*ARGSUSED*/
static int
finddeflim(const char *name, kva_t *kva, void *ctxt, void *pres)
{
	deflim_t *pdef = pres;
	char *val;

	if (pdef->def == NULL) {
		val = kva_match(kva, USERATTR_DFLTPRIV_KW);
		if (val != NULL)
			pdef->def = strdup(val);
	}
	if (pdef->lim == NULL) {
		val = kva_match(kva, USERATTR_LIMPRIV_KW);
		if (val != NULL)
			pdef->lim = strdup(val);
	}
	return (pdef->lim != NULL && pdef->def != NULL);
}

/*
 *	unix_cred - pam_sm_setcred
 *
 *	Entry flags = 	PAM_ESTABLISH_CRED, set up Solaris Unix cred.
 *			PAM_DELETE_CRED, NOP, return PAM_SUCCESS.
 *			PAM_REINITIALIZE_CRED, set up Solaris Unix cred,
 *				or merge the current context with the new
 *				user.
 *			PAM_REFRESH_CRED, set up Solaris Unix cred.
 *			PAM_SILENT, print no messages to user.
 *
 *	Returns	PAM_SUCCESS, if all successful.
 *		PAM_CRED_ERR, if unable to set credentials.
 *		PAM_USER_UNKNOWN, if PAM_USER not set, or unable to find
 *			user in databases.
 *		PAM_SYSTEM_ERR, if no valid flag.
 */

int
pam_sm_setcred(pam_handle_t *pamh, int flags, int argc, const char **argv)
{
	int	i;
	int	debug = 0;
	uint_t	nowarn = flags & PAM_SILENT;
	int	ret = PAM_SUCCESS;
	char	*user;
	char	*auser;
	char	*rhost;
	char	*tty;
	priv_set_t	*lim, *def, *tset;
	char		messages[PAM_MAX_NUM_MSG][PAM_MAX_MSG_SIZE];
	char		buf[PROJECT_BUFSZ];
	struct project	proj, *pproj;
	int		error;
	char		*projname;
	char		*kvs;
	struct passwd	pwd;
	struct passwd	*pwdp;
	char		pwbuf[NSS_BUFLEN_PASSWD];
	deflim_t	deflim;

	for (i = 0; i < argc; i++) {
		if (strcmp(argv[i], "debug") == 0)
			debug = 1;
		else if (strcmp(argv[i], "nowarn") == 0)
			nowarn |= 1;
	}

	if (debug)
		syslog(LOG_AUTH | LOG_DEBUG,
		    "pam_unix_cred: pam_sm_setcred(flags = %x, argc= %d)",
		    flags, argc);

	(void) pam_get_item(pamh, PAM_USER, (void **)&user);

	if (user == NULL || *user == '\0') {
		syslog(LOG_AUTH | LOG_ERR,
		    "pam_unix_cred: USER NULL or empty!\n");
		return (PAM_USER_UNKNOWN);
	}
	(void) pam_get_item(pamh, PAM_AUSER, (void **)&auser);
	(void) pam_get_item(pamh, PAM_RHOST, (void **)&rhost);
	(void) pam_get_item(pamh, PAM_TTY, (void **)&tty);
	if (debug)
		syslog(LOG_AUTH | LOG_DEBUG,
		    "pam_unix_cred: user = %s, auser = %s, rhost = %s, "
		    "tty = %s", user,
		    (auser == NULL) ? "NULL" : (*auser == '\0') ? "ZERO" :
		    auser,
		    (rhost == NULL) ? "NULL" : (*rhost == '\0') ? "ZERO" :
		    rhost,
		    (tty == NULL) ? "NULL" : (*tty == '\0') ? "ZERO" :
		    tty);

	/* validate flags */
	switch (flags & (PAM_ESTABLISH_CRED | PAM_DELETE_CRED |
	    PAM_REINITIALIZE_CRED | PAM_REFRESH_CRED)) {
	case 0:
		/* set default flag */
		flags |= PAM_ESTABLISH_CRED;
		break;
	case PAM_ESTABLISH_CRED:
	case PAM_REINITIALIZE_CRED:
	case PAM_REFRESH_CRED:
		break;
	case PAM_DELETE_CRED:
		return (PAM_SUCCESS);
	default:
		syslog(LOG_AUTH | LOG_ERR,
		    "pam_unix_cred: invalid flags %x", flags);
		return (PAM_SYSTEM_ERR);
	}

	getpwnam_r(user, &pwd, pwbuf, sizeof (pwbuf), &pwdp);
	if (!pwdp) {
		syslog(LOG_AUTH | LOG_ERR,
		    "pam_unix_cred: cannot get passwd entry for user = %s",
		    user);
		return (PAM_USER_UNKNOWN);
	}

	/* Initialize the user's project */
	(void) pam_get_item(pamh, PAM_RESOURCE, (void **)&kvs);
	if (kvs != NULL) {
		char *tmp, *lasts, *tok;

		kvs = tmp = strdup(kvs);
		if (kvs == NULL)
			return (PAM_BUF_ERR);

		while ((tok = strtok_r(tmp, ";", &lasts)) != NULL) {
			if (strncmp(tok, PROJECT, PROJSZ) == 0) {
				projname = tok + PROJSZ;
				break;
			}
			tmp = NULL;
		}
	} else {
		projname = NULL;
	}

	if (projname == NULL || *projname == '\0') {
		pproj = getdefaultproj(user, &proj, (void *)&buf,
		    PROJECT_BUFSZ);
	} else {
		pproj = getprojbyname(projname, &proj, (void *)&buf,
		    PROJECT_BUFSZ);
	}
	/* projname points into kvs, so this is the first opportunity to free */
	free(kvs);
	if (pproj == NULL) {
		syslog(LOG_AUTH | LOG_ERR,
		    "pam_unix_cred: no default project for user %s", user);
		if (!nowarn) {
			(void) snprintf(messages[0], sizeof (messages[0]),
			    dgettext(TEXT_DOMAIN, "No default project!"));
			(void) __pam_display_msg(pamh, PAM_ERROR_MSG,
			    1, messages, NULL);
		}
		return (PAM_SYSTEM_ERR);
	}
	if ((error = setproject(proj.pj_name, user, TASK_NORMAL)) != 0) {
		kva_t *kv_array;

		switch (error) {
		case SETPROJ_ERR_TASK:
			if (errno == EAGAIN) {
				syslog(LOG_AUTH | LOG_ERR,
				    "pam_unix_cred: project \"%s\" resource "
				    "control limit has been reached",
				    proj.pj_name);
				(void) snprintf(messages[0],
				    sizeof (messages[0]), dgettext(
				    TEXT_DOMAIN,
				    "Resource control limit has been "
				    "reached"));
			} else {
				syslog(LOG_AUTH | LOG_ERR,
				    "pam_unix_cred: user %s could not join "
				    "project \"%s\": %m", user, proj.pj_name);
				(void) snprintf(messages[0],
				    sizeof (messages[0]), dgettext(
				    TEXT_DOMAIN,
				    "Could not join default project"));
			}
			if (!nowarn)
				(void) __pam_display_msg(pamh, PAM_ERROR_MSG, 1,
				    messages, NULL);
			break;
		case SETPROJ_ERR_POOL:
			(void) snprintf(messages[0], sizeof (messages[0]),
			    dgettext(TEXT_DOMAIN,
			    "Could not bind to resource pool"));
			switch (errno) {
			case EACCES:
				syslog(LOG_AUTH | LOG_ERR,
				    "pam_unix_cred: project \"%s\" could not "
				    "bind to resource pool: No resource pool "
				    "accepting default bindings exists",
				    proj.pj_name);
				(void) snprintf(messages[1],
				    sizeof (messages[1]),
				    dgettext(TEXT_DOMAIN,
				    "No resource pool accepting "
				    "default bindings exists"));
				break;
			case ESRCH:
				syslog(LOG_AUTH | LOG_ERR,
				    "pam_unix_cred: project \"%s\" could not "
				    "bind to resource pool: The resource pool "
				    "is unknown", proj.pj_name);
				(void) snprintf(messages[1],
				    sizeof (messages[1]),
				    dgettext(TEXT_DOMAIN,
				    "The specified resource pool "
				    "is unknown"));
				break;
			default:
				(void) snprintf(messages[1],
				    sizeof (messages[1]),
				    dgettext(TEXT_DOMAIN,
				    "Failure during pool binding"));
				syslog(LOG_AUTH | LOG_ERR,
				    "pam_unix_cred: project \"%s\" could not "
				    "bind to resource pool: %m", proj.pj_name);
			}
			if (!nowarn)
				(void) __pam_display_msg(pamh, PAM_ERROR_MSG,
				    2, messages, NULL);
			break;
		default:
			/*
			 * Resource control assignment failed.  Unlike
			 * newtask(1m), we treat this as an error.
			 */
			if (error < 0) {
				/*
				 * This isn't supposed to happen, but in
				 * case it does, this error message
				 * doesn't use error as an index, like
				 * the others might.
				 */
				syslog(LOG_AUTH | LOG_ERR,
				    "pam_unix_cred: unkwown error joining "
				    "project \"%s\" (%d)", proj.pj_name, error);
				(void) snprintf(messages[0],
				    sizeof (messages[0]),
				    dgettext(TEXT_DOMAIN,
				    "unkwown error joining project \"%s\""
				    " (%d)"), proj.pj_name, error);
			} else if ((kv_array = _str2kva(proj.pj_attr, KV_ASSIGN,
			    KV_DELIMITER)) != NULL) {
				syslog(LOG_AUTH | LOG_ERR,
				    "pam_unix_cred: %s resource control "
				    "assignment failed for project \"%s\"",
				    kv_array->data[error - 1].key,
				    proj.pj_name);
				(void) snprintf(messages[0],
				    sizeof (messages[0]),
				    dgettext(TEXT_DOMAIN,
				    "%s resource control assignment failed for "
				    "project \"%s\""),
				    kv_array->data[error - 1].key,
				    proj.pj_name);
				_kva_free(kv_array);
			} else {
				syslog(LOG_AUTH | LOG_ERR,
				    "pam_unix_cred: resource control "
				    "assignment failed for project \"%s\""
				    "attribute %d", proj.pj_name, error);
				(void) snprintf(messages[0],
				    sizeof (messages[0]),
				    dgettext(TEXT_DOMAIN,
				    "resource control assignment failed for "
				    "project \"%s\" attribute %d"),
				    proj.pj_name, error);
			}
			if (!nowarn)
				(void) __pam_display_msg(pamh, PAM_ERROR_MSG,
				    1, messages, NULL);
		}
		return (PAM_SYSTEM_ERR);
	}

	tset = def = lim = NULL;
	deflim.def = deflim.lim = NULL;

	(void) _enum_attrs(user, finddeflim, NULL, &deflim);

	if (getset(deflim.lim, &lim) != 0 || getset(deflim.def, &def) != 0) {
		ret = PAM_SYSTEM_ERR;
		goto out;
	}

	if (def == NULL) {
		def = priv_allocset();
		if (def == NULL) {
			ret = PAM_SYSTEM_ERR;
			goto out;
		}
		priv_basicset(def);
		errno = 0;
		if ((pathconf("/", _PC_CHOWN_RESTRICTED) == -1) && (errno == 0))
			(void) priv_addset(def, PRIV_FILE_CHOWN_SELF);
	}
	/*
	 * Silently limit the privileges to those actually available
	 * in the current zone.
	 */
	tset = priv_allocset();
	if (tset == NULL) {
		ret = PAM_SYSTEM_ERR;
		goto out;
	}
	if (getppriv(PRIV_PERMITTED, tset) != 0) {
		ret = PAM_SYSTEM_ERR;
		goto out;
	}
	if (!priv_issubset(def, tset))
		priv_intersect(tset, def);
	/*
	 * We set privilege awareness here so that I gets copied to
	 * P & E when the final setuid(uid) happens.
	 */
	(void) setpflags(PRIV_AWARE, 1);
	if (setppriv(PRIV_SET, PRIV_INHERITABLE, def) != 0) {
		syslog(LOG_AUTH | LOG_ERR,
		    "pam_setcred: setppriv(defaultpriv) failed: %m");
		ret = PAM_CRED_ERR;
	}

	if (lim != NULL) {
		/*
		 * Silently limit the privileges to the limit set available.
		 */
		if (getppriv(PRIV_LIMIT, tset) != 0) {
			ret = PAM_SYSTEM_ERR;
			goto out;
		}
		if (!priv_issubset(lim, tset))
			priv_intersect(tset, lim);
		if (setppriv(PRIV_SET, PRIV_LIMIT, lim) != 0) {
			syslog(LOG_AUTH | LOG_ERR,
			    "pam_setcred: setppriv(limitpriv) failed: %m");
			ret = PAM_CRED_ERR;
			goto out;
		}
		/*
		 * In order not to surprise certain applications, we
		 * need to get rid of privilege awareness and thus we must
		 * set this flag which will cause a reset on set*uid().
		 */
		(void) setpflags(PRIV_AWARE_RESET, 1);
	}
	/*
	 * This may fail but we do not care as this will be reset later
	 * when the uids are set to their final values.
	 */
	(void) setpflags(PRIV_AWARE, 0);
	/*
	 * Remove PRIV_PFEXEC; stop running as if we are under a profile
	 * shell.  A user with a profile shell will set PRIV_PFEXEC.
	 */
	(void) setpflags(PRIV_PFEXEC, 0);

out:
	free(deflim.lim);
	free(deflim.def);

	if (lim != NULL)
		priv_freeset(lim);
	if (def != NULL)
		priv_freeset(def);
	if (tset != NULL)
		priv_freeset(tset);

	return (ret);
}
