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
 * Copyright (c) 1989, 2010, Oracle and/or its affiliates. All rights reserved.
 */

/*	Copyright (c) 1984, 1986, 1987, 1988, 1989 AT&T	*/
/*	  All Rights Reserved  	*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <string.h>
#include <memory.h>
#include <sys/types.h>
#include <signal.h>
#include <libproc.h>
#include "ramdata.h"
#include "systable.h"
#include "proto.h"

/* XXX A bug in the <string.h> header file requires this */
extern char *strtok_r(char *s1, const char *s2, char **lasts);

/*
 * option procesing ---
 * Routines for scanning syscall, signal, fault
 * and file descriptor lists.
 */

/*
 * Function prototypes for static routines in this module.
 */
void	upcase(char *);

const char white[] = " \t\n";	/* white space characters */
const char sepr[] = " ,\t\n";	/* list separator characters */
const char csepr[] = " :,\t\n";	/* same, with ':' added */

/*
 * Scan list of syscall names.
 * Return 0 on success, != 0 on any failure.
 */
int
syslist(char *str,			/* string of syscall names */
	sysset_t *setp,			/* syscall set */
	int *fp)			/* first-time flag */
{
	char *name;
	int exclude = FALSE;
	int rc = 0;
	char *lasts;

	name = strtok_r(str, sepr, &lasts);

	if (name != NULL && *name == '!') {	/* exclude from set */
		exclude = TRUE;
		if (*++name == '\0')
			name = strtok_r(NULL, sepr, &lasts);
	} else if (!*fp) {	/* first time, clear the set */
		premptyset(setp);
		*fp = TRUE;
	}

	for (; name; name = strtok_r(NULL, sepr, &lasts)) {
		int sys;
		int sysx;
		char *next;

		if (*name == '!') {	/* exclude remainder from set */
			exclude = TRUE;
			while (*++name == '!')
				/* empty */;
			if (*name == '\0')
				continue;
		}

		sys = strtol(name, &next, 0);
		sysx = 0;
		if (sys < 0 || sys > PRMAXSYS || *next != '\0')
			sys = 0;
		if (sys == 0) {
			const struct systable *stp = systable;
			for (; sys == 0 && stp->nargs >= 0; stp++)
				if (stp->name && strcmp(stp->name, name) == 0)
					sys = stp-systable;
		}
		if (sys == 0) {
			const struct sysalias *sap = sysalias;
			for (; sys == 0 && sap->name; sap++)
				if (strcmp(sap->name, name) == 0)
					sys = sap->number;
		}
		if (sys > 0 && sys <= PRMAXSYS) {
			switch (sys) {
			case SYS_fstatat:
				sys = SYS_fstatat;
				goto def;

			case SYS_stat:
				sys = SYS_stat;
				sysx = SYS_fstatat;
				goto def;

			case SYS_lstat:
				sys = SYS_lstat;
				sysx = SYS_fstatat;
				goto def;

			case SYS_fstat:
				sys = SYS_fstat;
				sysx = SYS_fstatat;
				goto def;

			case SYS_getdents:
				sys = SYS_getdents;
				goto def;

			case SYS_mmap:
				sys = SYS_mmap;
				goto def;

			case SYS_statvfs:
				sys = SYS_statvfs;
				goto def;

			case SYS_fstatvfs:
				sys = SYS_fstatvfs;
				goto def;

			case SYS_setrlimit:
				sys = SYS_setrlimit;
				goto def;

			case SYS_getrlimit:
				sys = SYS_getrlimit;
				goto def;

			case SYS_pread:
				sys = SYS_pread;
				goto def;

			case SYS_pwrite:
				sys = SYS_pwrite;
				goto def;

			case SYS_openat:
			case SYS_open:
				sys = SYS_openat;
				sysx = SYS_open;
				goto def;

			case SYS_forksys:	/* set both if either */
			case SYS_vfork:
				sysx = SYS_forksys;
				sys = SYS_vfork;
				goto def;

			case SYS_sigprocmask:	/* set both if either */
			case SYS_lwp_sigmask:
				sysx = SYS_sigprocmask;
				sys = SYS_lwp_sigmask;
				goto def;

			case SYS_lseek:		/* set both if either */
			case SYS_llseek:
				sysx = SYS_lseek;
				sys = SYS_llseek;
				goto def;

			case SYS_rename:	/* set both */
				sysx = SYS_renameat;
				goto def;

			case SYS_link:		/* set both */
				sysx = SYS_linkat;
				goto def;

			case SYS_unlink:	/* set both */
			case SYS_rmdir:		/* set both */
				sysx = SYS_unlinkat;
				goto def;

			case SYS_symlink:	/* set both */
				sysx = SYS_symlinkat;
				goto def;

			case SYS_readlink:	/* set both */
				sysx = SYS_readlinkat;
				goto def;

			case SYS_chmod:		/* set both */
			case SYS_fchmod:	/* set both */
				sysx = SYS_fchmodat;
				goto def;

			case SYS_chown:		/* set both */
			case SYS_lchown:	/* set both */
			case SYS_fchown:	/* set both */
				sysx = SYS_fchownat;
				goto def;

			case SYS_mkdir:		/* set both */
				sysx = SYS_mkdirat;
				goto def;

			case SYS_mknod:		/* set both */
				sysx = SYS_mknodat;
				goto def;

			case SYS_access:	/* set both */
				sysx = SYS_faccessat;
				goto def;

			default:
			def:
				if (exclude) {
					prdelset(setp, sys);
					if (sysx)
						prdelset(setp, sysx);
				} else {
					praddset(setp, sys);
					if (sysx)
						praddset(setp, sysx);
				}
				break;
			}
		} else if (strcmp(name, "all") == 0 ||
		    strcmp(name, "ALL") == 0) {
			if (exclude) {
				premptyset(setp);
			} else {
				prfillset(setp);
			}
		} else {
			(void) fprintf(stderr,
			    "%s: unrecognized syscall: %s\n",
			    command, name);
			rc = -1;
		}
	}

	return (rc);
}

/*
 * List of signals to trace.
 * Return 0 on success, != 0 on any failure.
 */
int
siglist(private_t *pri,
	char *str,			/* string of signal names */
	sigset_t *setp,			/* signal set */
	int *fp)			/* first-time flag */
{
	char *name;
	int exclude = FALSE;
	int rc = 0;
	char *lasts;

	upcase(str);
	name = strtok_r(str, sepr, &lasts);

	if (name != NULL && *name == '!') {	/* exclude from set */
		exclude = TRUE;
		if (*++name == '\0')
			name = strtok_r(NULL, sepr, &lasts);
	} else if (!*fp) {	/* first time, clear the set */
		premptyset(setp);
		*fp = TRUE;
	}

	for (; name; name = strtok_r(NULL, sepr, &lasts)) {
		int sig;
		char *next;

		if (*name == '!') {	/* exclude remainder from set */
			exclude = TRUE;
			while (*++name == '!')
				/* empty */;
			if (*name == '\0')
				continue;
		}

		sig = strtol(name, &next, 0);
		if (sig <= 0 || sig > PRMAXSIG || *next != '\0') {
			for (sig = 1; sig <= PRMAXSIG; sig++) {
				const char *sname = rawsigname(pri, sig);
				if (sname == NULL)
					continue;
				if (strcmp(sname, name) == 0 ||
				    strcmp(sname+3, name) == 0)
					break;
			}
			if (sig > PRMAXSIG)
				sig = 0;
		}
		if (sig > 0 && sig <= PRMAXSIG) {
			if (exclude) {
				prdelset(setp, sig);
			} else {
				praddset(setp, sig);
			}
		} else if (strcmp(name, "ALL") == 0) {
			if (exclude) {
				premptyset(setp);
			} else {
				prfillset(setp);
			}
		} else {
			(void) fprintf(stderr,
			    "%s: unrecognized signal name/number: %s\n",
			    command, name);
			rc = -1;
		}
	}

	return (rc);
}

/*
 * List of faults to trace.
 * return 0 on success, != 0 on any failure.
 */
int
fltlist(char *str,			/* string of fault names */
	fltset_t *setp,			/* fault set */
	int *fp)			/* first-time flag */
{
	char *name;
	int exclude = FALSE;
	int rc = 0;
	char *lasts;

	upcase(str);
	name = strtok_r(str, sepr, &lasts);

	if (name != NULL && *name == '!') {	/* exclude from set */
		exclude = TRUE;
		if (*++name == '\0')
			name = strtok_r(NULL, sepr, &lasts);
	} else if (!*fp) {	/* first time, clear the set */
		premptyset(setp);
		*fp = TRUE;
	}

	for (; name; name = strtok_r(NULL, sepr, &lasts)) {
		int flt;
		char *next;

		if (*name == '!') {	/* exclude remainder from set */
			exclude = TRUE;
			while (*++name == '!')
				/* empty */;
			if (*name == '\0')
				continue;
		}

		flt = strtol(name, &next, 0);
		if (flt <= 0 || flt > PRMAXFAULT || *next != '\0') {
			for (flt = 1; flt <= PRMAXFAULT; flt++) {
				char fname[32];

				if (proc_fltname(flt, fname,
				    sizeof (fname)) == NULL)
					continue;

				if (strcmp(fname, name) == 0 ||
				    strcmp(fname+3, name) == 0)
					break;
			}
			if (flt > PRMAXFAULT)
				flt = 0;
		}
		if (flt > 0 && flt <= PRMAXFAULT) {
			if (exclude) {
				prdelset(setp, flt);
			} else {
				praddset(setp, flt);
			}
		} else if (strcmp(name, "ALL") == 0) {
			if (exclude) {
				premptyset(setp);
			} else {
				prfillset(setp);
			}
		} else {
			(void) fprintf(stderr,
			    "%s: unrecognized fault name/number: %s\n",
			    command, name);
			rc = -1;
		}
	}

	return (rc);
}

/*
 * Gather file descriptors to dump.
 * Return 0 on success, != 0 on any failure.
 */
int
fdlist(char *str,		/* string of filedescriptors */
	fileset_t *setp)	/* set of boolean flags */
{
	char *name;
	int exclude = FALSE;
	int rc = 0;
	char *lasts;

	upcase(str);
	name = strtok_r(str, sepr, &lasts);

	if (name != NULL && *name == '!') {	/* exclude from set */
		exclude = TRUE;
		if (*++name == '\0')
			name = strtok_r(NULL, sepr, &lasts);
	}

	for (; name; name = strtok_r(NULL, sepr, &lasts)) {
		int fd;
		char *next;

		if (*name == '!') {	/* exclude remainder from set */
			exclude = TRUE;
			while (*++name == '!')
				/* empty */;
			if (*name == '\0')
				continue;
		}

		fd = strtol(name, &next, 0);
		if (fd >= 0 && fd < NOFILES_MAX && *next == '\0') {
			fd++;
			if (exclude) {
				prdelset(setp, fd);
			} else {
				praddset(setp, fd);
			}
		} else if (strcmp(name, "ALL") == 0) {
			if (exclude) {
				premptyset(setp);
			} else {
				prfillset(setp);
			}
		} else {
			(void) fprintf(stderr,
			    "%s: filedescriptor not in range[0..%d]: %s\n",
			    command, NOFILES_MAX-1, name);
			rc = -1;
		}
	}

	return (rc);
}

void
upcase(char *str)
{
	int c;

	while ((c = *str) != '\0')
		*str++ = toupper(c);
}

/*
 * 'arg' points to a string like:
 *	libc,libnsl,... : printf,read,write,...
 * or
 *	libc,libnsl,... :: printf,read,write,...
 * with possible filename pattern-matching metacharacters.
 *
 * Assumption:  No library or function name can contain ',' or ':'.
 */
int
liblist(char *arg, int hang)
{
	const char *star = "*";
	struct dynpat *Dyp;
	char *pat;
	char *fpat;
	char *lasts;
	uint_t maxpat;

	/* append a new dynpat structure to the end of the Dynpat list */
	Dyp = my_malloc(sizeof (struct dynpat), NULL);
	Dyp->next = NULL;
	if (Lastpat == NULL)
		Dynpat = Lastpat = Dyp;
	else {
		Lastpat->next = Dyp;
		Lastpat = Dyp;
	}
	Dyp->flag = hang? BPT_HANG : 0;
	Dyp->exclude_lib = 0;
	Dyp->exclude = 0;
	Dyp->internal = 0;
	Dyp->Dp = NULL;

	/*
	 * Find the beginning of the filename patterns
	 * and null-terminate the library name patterns.
	 */
	if ((fpat = strchr(arg, ':')) != NULL)
		*fpat++ = '\0';

	/*
	 * Library name patterns.
	 */
	pat = strtok_r(arg, sepr, &lasts);

	/* '!' introduces an exclusion list */
	if (pat != NULL && *pat == '!') {
		Dyp->exclude_lib = 1;
		pat += strspn(pat, "!");
		if (*pat == '\0')
			pat = strtok_r(NULL, sepr, &lasts);
		/* force exclusion of all functions as well */
		Dyp->exclude = 1;
		Dyp->internal = 1;
		fpat = NULL;
	}

	if (pat == NULL) {
		/* empty list means all libraries */
		Dyp->libpat = my_malloc(sizeof (char *), NULL);
		Dyp->libpat[0] = star;
		Dyp->nlibpat = 1;
	} else {
		/*
		 * We are now at the library list.
		 * Generate the list and count the library name patterns.
		 */
		maxpat = 1;
		Dyp->libpat = my_malloc(maxpat * sizeof (char *), NULL);
		Dyp->nlibpat = 0;
		Dyp->libpat[Dyp->nlibpat++] = pat;
		while ((pat = strtok_r(NULL, sepr, &lasts)) != NULL) {
			if (Dyp->nlibpat == maxpat) {
				maxpat *= 2;
				Dyp->libpat = my_realloc(Dyp->libpat,
				    maxpat * sizeof (char *), NULL);
			}
			Dyp->libpat[Dyp->nlibpat++] = pat;
		}
	}

	/*
	 * Function name patterns.
	 */
	if (fpat == NULL)
		pat = NULL;
	else {
		/*
		 * We have already seen a ':'.  Look for another.
		 * Double ':' means trace internal calls.
		 */
		fpat += strspn(fpat, white);
		if (*fpat == ':') {
			Dyp->internal = 1;
			*fpat++ = '\0';
		}
		pat = strtok_r(fpat, csepr, &lasts);
	}

	/* '!' introduces an exclusion list */
	if (pat != NULL && *pat == '!') {
		Dyp->exclude = 1;
		Dyp->internal = 1;
		pat += strspn(pat, "!");
		if (*pat == '\0')
			pat = strtok_r(NULL, sepr, &lasts);
	}

	if (pat == NULL) {
		/* empty function list means exclude all functions */
		Dyp->sympat = my_malloc(sizeof (char *), NULL);
		Dyp->sympat[0] = star;
		Dyp->nsympat = 1;
	} else {
		/*
		 * We are now at the function list.
		 * Generate the list and count the symbol name patterns.
		 */
		maxpat = 1;
		Dyp->sympat = my_malloc(maxpat * sizeof (char *), NULL);
		Dyp->nsympat = 0;
		Dyp->sympat[Dyp->nsympat++] = pat;
		while ((pat = strtok_r(NULL, sepr, &lasts)) != NULL) {
			if (Dyp->nsympat == maxpat) {
				maxpat *= 2;
				Dyp->sympat = my_realloc(Dyp->sympat,
				    maxpat * sizeof (char *), NULL);
			}
			Dyp->sympat[Dyp->nsympat++] = pat;
		}
	}

	return (0);
}
