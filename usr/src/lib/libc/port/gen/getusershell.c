/*
 * Copyright (c) 1990, 2010, Oracle and/or its affiliates. All rights reserved.
 */

/*	Copyright (c) 1984, 1986, 1987, 1988, 1989 AT&T	*/
/*	  All Rights Reserved  	*/

/*
 * Copyright (c) 1985 Regents of the University of California.
 * All rights reserved.  The Berkeley software License Agreement
 * specifies the terms and conditions for redistribution.
 */

#include "lint.h"
#include <sys/types.h>
#include <sys/param.h>
#include <sys/stat.h>
#include <ctype.h>
#include <stdio.h>
#include <limits.h>
#include <stdlib.h>
#include <sys/file.h>
#include "libc.h"
#include <unistd.h>

#define	SHELLS "/etc/shells"

/*
 * Do not add local shells here.  They should be added in /etc/shells
 *
 * Do not add restricted shells:
 * Shells returned by getusershell traditionally allow:
 * - users to change away from (i.e., if you have an rksh in
 *   getusershell(), then users can change their shell to ksh)
 * - by default, ftp in is allowed only for shells returned by
 *   getusershell(); since FTP has no restrictions on directory
 *   movement, adding rksh to getusershell() would defeat that
 *   protection.
 */
const char *okshells[] = {
	"/usr/bin/sh",
	"/usr/bin/ksh",
	"/usr/bin/jsh",
	"/bin/sh",
	"/bin/ksh",
	"/bin/jsh",
	"/usr/bin/pfsh",
	"/usr/bin/pfksh",
	"/usr/bin/bash",
	"/usr/bin/zsh",
	"/usr/bin/pfbash",
	"/usr/bin/pfzsh",
	"/bin/pfsh",
	"/bin/pfksh",
	"/bin/bash",
	"/bin/zsh",
	"/bin/pfbash",
	"/bin/pfzsh",
	"/usr/sfw/bin/zsh",
	NULL
};

static char **shells, *strings;
static char **curshell;
static char **initshells(void);

/*
 * Get a list of shells from SHELLS, if it exists.
 */
char *
getusershell(void)
{
	char *ret;

	if (curshell == NULL)
		curshell = initshells();
	ret = *curshell;
	if (ret != NULL)
		curshell++;
	return (ret);
}

void
endusershell(void)
{

	if (shells != NULL)
		(void) free((char *)shells);
	shells = NULL;
	if (strings != NULL)
		(void) free(strings);
	strings = NULL;
	curshell = NULL;
}

void
setusershell(void)
{

	curshell = initshells();
}

static char **
initshells(void)
{
	char **sp, *cp;
	FILE *fp;
	struct stat statb;

	if (shells != NULL)
		(void) free((char *)shells);
	shells = NULL;
	if (strings != NULL)
		(void) free(strings);
	strings = NULL;
	if ((fp = fopen(SHELLS, "rF")) == (FILE *)0)
		return ((char **)okshells);
	/*
	 * The +1 in the malloc() below is needed to handle the final
	 * fgets() NULL terminator.  From fgets(3S):
	 *
	 * char *fgets(char *s, int n, FILE *stream);
	 *
	 * The  fgets()  function reads characters from the stream into
	 * the array pointed to by s, until n-1 characters are read, or
	 * a newline character is read and transferred to s, or an end-
	 * of-file condition is encountered.  The string is then termi-
	 * nated with a null character.
	 */
	if ((fstat(fileno(fp), &statb) == -1) || (statb.st_size > LONG_MAX) ||
	    ((strings = malloc((size_t)statb.st_size + 1)) == NULL)) {
		(void) fclose(fp);
		return ((char **)okshells);
	}
	shells = calloc((size_t)statb.st_size / 3, sizeof (char *));
	if (shells == NULL) {
		(void) fclose(fp);
		(void) free(strings);
		strings = NULL;
		return ((char **)okshells);
	}
	sp = shells;
	cp = strings;
	while (fgets(cp, MAXPATHLEN + 1, fp) != NULL) {
		while (*cp != '#' && *cp != '/' && *cp != '\0')
			cp++;
		if (*cp == '#' || *cp == '\0')
			continue;
		*sp++ = cp;
		while (!isspace(*cp) && *cp != '#' && *cp != '\0')
			cp++;
		*cp++ = '\0';
	}
	*sp = NULL;
	(void) fclose(fp);
	return (shells);
}
