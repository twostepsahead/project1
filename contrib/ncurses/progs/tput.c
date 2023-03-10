/****************************************************************************
 * Copyright (c) 1998-2016,2017 Free Software Foundation, Inc.              *
 *                                                                          *
 * Permission is hereby granted, free of charge, to any person obtaining a  *
 * copy of this software and associated documentation files (the            *
 * "Software"), to deal in the Software without restriction, including      *
 * without limitation the rights to use, copy, modify, merge, publish,      *
 * distribute, distribute with modifications, sublicense, and/or sell       *
 * copies of the Software, and to permit persons to whom the Software is    *
 * furnished to do so, subject to the following conditions:                 *
 *                                                                          *
 * The above copyright notice and this permission notice shall be included  *
 * in all copies or substantial portions of the Software.                   *
 *                                                                          *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS  *
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF               *
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.   *
 * IN NO EVENT SHALL THE ABOVE COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,   *
 * DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR    *
 * OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR    *
 * THE USE OR OTHER DEALINGS IN THE SOFTWARE.                               *
 *                                                                          *
 * Except as contained in this notice, the name(s) of the above copyright   *
 * holders shall not be used in advertising or otherwise to promote the     *
 * sale, use or other dealings in this Software without prior written       *
 * authorization.                                                           *
 ****************************************************************************/

/****************************************************************************
 *  Author: Zeyd M. Ben-Halim <zmbenhal@netcom.com> 1992,1995               *
 *     and: Eric S. Raymond <esr@snark.thyrsus.com>                         *
 *     and: Thomas E. Dickey                        1996-on                 *
 ****************************************************************************/

/*
 * tput.c -- shellscript access to terminal capabilities
 *
 * by Eric S. Raymond <esr@snark.thyrsus.com>, portions based on code from
 * Ross Ridge's mytinfo package.
 */

#include <tparm_type.h>
#include <clear_cmd.h>
#include <reset_cmd.h>

#if !PURE_TERMINFO
#include <dump_entry.h>
#include <termsort.c>
#endif
#include <transform.h>
#include <tty_settings.h>

MODULE_ID("$Id: tput.c,v 1.77 2017/10/07 23:51:01 tom Exp $")

#define PUTS(s)		fputs(s, stdout)

const char *_nc_progname = "tput";

static char *prg_name;
static bool is_init = FALSE;
static bool is_reset = FALSE;
static bool is_clear = FALSE;

static void
quit(int status, const char *fmt,...)
{
    va_list argp;

    va_start(argp, fmt);
    fprintf(stderr, "%s: ", prg_name);
    vfprintf(stderr, fmt, argp);
    fprintf(stderr, "\n");
    va_end(argp);
    ExitProgram(status);
}

static void
usage(void)
{
#define KEEP(s) s "\n"
    static const char msg[] =
    {
	KEEP("")
	KEEP("Options:")
	KEEP("  -S <<       read commands from standard input")
	KEEP("  -T TERM     use this instead of $TERM")
	KEEP("  -V          print curses-version")
	KEEP("  -x          do not try to clear scrollback")
	KEEP("")
	KEEP("Commands:")
	KEEP("  clear       clear the screen")
	KEEP("  init        initialize the terminal")
	KEEP("  reset       reinitialize the terminal")
	KEEP("  capname     unlike clear/init/reset, print value for capability \"capname\"")
    };
#undef KEEP
    (void) fprintf(stderr, "Usage: %s [options] [command]\n", prg_name);
    fputs(msg, stderr);
    ExitProgram(ErrUsage);
}

static char *
check_aliases(char *name, bool program)
{
    static char my_init[] = "init";
    static char my_reset[] = "reset";
    static char my_clear[] = "clear";

    char *result = name;
    if ((is_init = same_program(name, program ? PROG_INIT : my_init)))
	result = my_init;
    if ((is_reset = same_program(name, program ? PROG_RESET : my_reset)))
	result = my_reset;
    if ((is_clear = same_program(name, program ? PROG_CLEAR : my_clear)))
	result = my_clear;
    return result;
}

static int
exit_code(int token, int value)
{
    int result = 99;

    switch (token) {
    case BOOLEAN:
	result = !value;	/* TRUE=0, FALSE=1 */
	break;
    case NUMBER:
	result = 0;		/* always zero */
	break;
    case STRING:
	result = value;		/* 0=normal, 1=missing */
	break;
    }
    return result;
}

/*
 * Returns nonzero on error.
 */
static int
tput_cmd(int fd, TTY * saved_settings, bool opt_x, int argc, char *argv[])
{
    NCURSES_CONST char *name;
    char *s;
    int status;
#if !PURE_TERMINFO
    bool termcap = FALSE;
#endif

    name = check_aliases(argv[0], FALSE);
    if (is_reset || is_init) {
	TTY oldmode;

	int terasechar = -1;	/* new erase character */
	int intrchar = -1;	/* new interrupt character */
	int tkillchar = -1;	/* new kill character */

	if (is_reset) {
	    reset_start(stdout, TRUE, FALSE);
	    reset_tty_settings(fd, saved_settings);
	} else {
	    reset_start(stdout, FALSE, TRUE);
	}

#if HAVE_SIZECHANGE
	set_window_size(fd, &lines, &columns);
#else
	(void) fd;
#endif
	set_control_chars(saved_settings, terasechar, intrchar, tkillchar);
	set_conversions(saved_settings);
	if (send_init_strings(fd, &oldmode)) {
	    reset_flush();
	}

	update_tty_settings(&oldmode, saved_settings);
	return 0;
    }

    if (strcmp(name, "longname") == 0) {
	PUTS(longname());
	return 0;
    }
#if !PURE_TERMINFO
  retry:
#endif
    if (strcmp(name, "clear") == 0) {
	return (clear_cmd(opt_x) == ERR) ? ErrUsage : 0;
    } else if ((status = tigetflag(name)) != -1) {
	return exit_code(BOOLEAN, status);
    } else if ((status = tigetnum(name)) != CANCELLED_NUMERIC) {
	(void) printf("%d\n", status);
	return exit_code(NUMBER, 0);
    } else if ((s = tigetstr(name)) == CANCELLED_STRING) {
#if !PURE_TERMINFO
	if (!termcap) {
	    const struct name_table_entry *np;

	    termcap = TRUE;
	    if ((np = _nc_find_entry(name, _nc_get_hash_table(termcap))) != 0) {
		switch (np->nte_type) {
		case BOOLEAN:
		    name = boolnames[np->nte_index];
		    break;

		case NUMBER:
		    name = numnames[np->nte_index];
		    break;

		case STRING:
		    name = strnames[np->nte_index];
		    break;
		}
		goto retry;
	    }
	}
#endif
	quit(ErrCapName, "unknown terminfo capability '%s'", name);
    } else if (VALID_STRING(s)) {
	if (argc > 1) {
	    int k;
	    int ignored;
	    long numbers[1 + NUM_PARM];
	    char *strings[1 + NUM_PARM];
	    char *p_is_s[NUM_PARM];

	    /* Nasty hack time. The tparm function needs to see numeric
	     * parameters as numbers, not as pointers to their string
	     * representations
	     */

	    for (k = 1; k < argc; k++) {
		char *tmp = 0;
		strings[k] = argv[k];
		numbers[k] = strtol(argv[k], &tmp, 0);
		if (tmp == 0 || *tmp != 0)
		    numbers[k] = 0;
	    }
	    for (k = argc; k <= NUM_PARM; k++) {
		numbers[k] = 0;
		strings[k] = 0;
	    }

	    switch (tparm_type(name)) {
	    case Num_Str:
		s = TPARM_2(s, numbers[1], strings[2]);
		break;
	    case Num_Str_Str:
		s = TPARM_3(s, numbers[1], strings[2], strings[3]);
		break;
	    case Numbers:
	    default:
		(void) _nc_tparm_analyze(s, p_is_s, &ignored);
#define myParam(n) (p_is_s[n - 1] != 0 ? ((TPARM_ARG) strings[n]) : numbers[n])
		s = TPARM_9(s,
			    myParam(1),
			    myParam(2),
			    myParam(3),
			    myParam(4),
			    myParam(5),
			    myParam(6),
			    myParam(7),
			    myParam(8),
			    myParam(9));
		break;
	    }
	}

	/* use putp() in order to perform padding */
	putp(s);
	return exit_code(STRING, 0);
    }
    return exit_code(STRING, 1);
}

int
main(int argc, char **argv)
{
    char *term;
    int errret;
    bool cmdline = TRUE;
    int c;
    char buf[BUFSIZ];
    int result = 0;
    int fd;
    TTY tty_settings;
    bool opt_x = FALSE;		/* clear scrollback if possible */
    bool is_alias;
    bool need_tty;

    prg_name = check_aliases(_nc_rootname(argv[0]), TRUE);

    term = getenv("TERM");

    while ((c = getopt(argc, argv, "ST:V")) != -1) {
	switch (c) {
	case 'S':
	    cmdline = FALSE;
	    break;
	case 'T':
	    use_env(FALSE);
	    use_tioctl(TRUE);
	    term = optarg;
	    break;
	case 'V':
	    puts(curses_version());
	    ExitProgram(EXIT_SUCCESS);
	case 'x':		/* do not try to clear scrollback */
	    opt_x = TRUE;
	    break;
	default:
	    usage();
	    /* NOTREACHED */
	}
    }

    is_alias = (is_clear || is_reset || is_init);
    need_tty = (is_reset || is_init);

    /*
     * Modify the argument list to omit the options we processed.
     */
    if (is_alias) {
	if (optind-- < argc) {
	    argc -= optind;
	    argv += optind;
	}
	argv[0] = prg_name;
    } else {
	argc -= optind;
	argv += optind;
    }

    if (term == 0 || *term == '\0')
	quit(ErrUsage, "No value for $TERM and no -T specified");

    fd = save_tty_settings(&tty_settings, need_tty);

    if (setupterm(term, fd, &errret) != OK && errret <= 0)
	quit(ErrTermType, "unknown terminal \"%s\"", term);

    if (cmdline) {
	if ((argc <= 0) && !is_alias)
	    usage();
	ExitProgram(tput_cmd(fd, &tty_settings, opt_x, argc, argv));
    }

    while (fgets(buf, sizeof(buf), stdin) != 0) {
	char *argvec[16];	/* command, 9 parms, null, & slop */
	int argnum = 0;
	char *cp;

	/* crack the argument list into a dope vector */
	for (cp = buf; *cp; cp++) {
	    if (isspace(UChar(*cp))) {
		*cp = '\0';
	    } else if (cp == buf || cp[-1] == 0) {
		argvec[argnum++] = cp;
		if (argnum >= (int) SIZEOF(argvec) - 1)
		    break;
	    }
	}
	argvec[argnum] = 0;

	if (argnum != 0
	    && tput_cmd(fd, &tty_settings, opt_x, argnum, argvec) != 0) {
	    if (result == 0)
		result = ErrSystem(0);	/* will return value >4 */
	    ++result;
	}
    }

    ExitProgram(result);
}
