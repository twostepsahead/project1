/*
 * Copyright (C) 1993-2001 by Darren Reed.
 *
 * See the IPFILTER.LICENCE file for details on licencing.
 *
 * Copyright 2008 Sun Microsystems, Inc.  All rights reserved.
 * Use is subject to license terms.
 *
 * Copyright (c) 2014, Joyent, Inc.  All rights reserved.
 */

#ifdef	__FreeBSD__
# ifndef __FreeBSD_cc_version
#  include <osreldate.h>
# else
#  if __FreeBSD_cc_version < 430000
#   include <osreldate.h>
#  endif
# endif
#endif
#include "ipf.h"
#include <fcntl.h>
#include <sys/ioctl.h>
#include "netinet/ipl.h"
#include "ipfzone.h"

static const char sccsid[] = "@(#)ipf.c	1.23 6/5/96 (C) 1993-2000 Darren Reed";
static const char rcsid[] = "@(#)$Id: ipf.c,v 1.35.2.3 2004/12/15 18:27:17 darrenr Exp $";

#if !defined(__SVR4) && defined(__GNUC__)
extern	char	*index __P((const char *, int));
#endif

extern	char	*optarg;
extern	int	optind;
extern	frentry_t *frtop;


void	ipf_frsync __P((void));
void	zerostats __P((void));
int	main __P((int, char *[]));

int	opts = 0;
int	outputc = 0;
int	use_inet6 = 0;

static	void	procfile __P((char *, char *)), flushfilter __P((char *));
static	void	set_state __P((u_int)), showstats __P((friostat_t *));
static	void	packetlogon __P((char *)), swapactive __P((void));
static	int	opendevice __P((char *, int));
static	void	closedevice __P((void));
static	char	*ipfname = IPL_NAME;
static	void	usage __P((void));
static	int	showversion __P((void));
static	int	get_flags __P((void));
static	void	ipf_interceptadd __P((int, ioctlfunc_t, void *));

static	int	fd = -1;
static	ioctlfunc_t	iocfunctions[IPL_LOGSIZE] = { ioctl, ioctl, ioctl,
						      ioctl, ioctl, ioctl,
						      ioctl, ioctl };


static void usage()
{
	fprintf(stderr, "usage: ipf [-6AdDEGInoPrRsvVyzZ] %s %s %s",
		"[-l block|pass|nomatch|state|nat]", "[-cc] [-F i|o|a|s|S|u]",
		"[-f filename] [-T <tuneopts>] [zonename]\n");
	exit(1);
}


int main(argc,argv)
int argc;
char *argv[];
{
	int c;
	const char *optstr = "6Ac:dDEf:F:GIl:noPrRsT:vVyzZ";

	if (argc < 2)
		usage();

	/*
	 * We need to set the zone name before calling the functions
	 * in the switch statement below. Note that ipf.c differs from the other
	 * tools in the ipfilter suite: the zone name is specified as the
	 * last argument, while the other tools use the -z option. ipf
	 * already has a -z option, so the last argument is used instead.
	 */
	getzonearg(argc, argv, optstr);

	while ((c = getopt(argc, argv, optstr)) != -1) {
		switch (c)
		{
		case '?' :
			usage();
			break;
#ifdef	USE_INET6
		case '6' :
			use_inet6 = 1;
			break;
#endif
		case 'A' :
			opts &= ~OPT_INACTIVE;
			break;
		case 'c' :
			if (strcmp(optarg, "c") == 0)
				outputc = 1;
			break;
		case 'E' :
			set_state((u_int)1);
			break;
		case 'D' :
			set_state((u_int)0);
			break;
		case 'd' :
			opts ^= OPT_DEBUG;
			break;
		case 'f' :
			procfile(argv[0], optarg);
			break;
		case 'F' :
			flushfilter(optarg);
			break;
		case 'G' :
			/* Already handled by getzonearg() above */
			break;
		case 'I' :
			opts ^= OPT_INACTIVE;
			break;
		case 'l' :
			packetlogon(optarg);
			break;
		case 'n' :
			opts ^= OPT_DONOTHING;
			break;
		case 'o' :
			break;
		case 'P' :
			ipfname = IPAUTH_NAME;
			break;
		case 'R' :
			opts ^= OPT_NORESOLVE;
			break;
		case 'r' :
			opts ^= OPT_REMOVE;
			break;
		case 's' :
			swapactive();
			break;
		case 'T' :
			if (opendevice(ipfname, 1) >= 0)
				ipf_dotuning(fd, optarg, ioctl);
			break;
		case 'v' :
			opts += OPT_VERBOSE;
			break;
		case 'V' :
			if (showversion())
				exit(1);
			break;
		case 'y' :
			ipf_frsync();
			break;
		case 'z' :
			opts ^= OPT_ZERORULEST;
			break;
		case 'Z' :
			zerostats();
			break;
		}
	}

	if (optind < 2)
		usage();

	if (fd != -1)
		(void) close(fd);

	return(0);
	/* NOTREACHED */
}


static int opendevice(ipfdev, check)
char *ipfdev;
int check;
{
	if (opts & OPT_DONOTHING)
		return -2;

	if (check && checkrev(ipfname) == -1) {
		fprintf(stderr, "User/kernel version check failed\n");
		return -2;
	}

	if (!ipfdev)
		ipfdev = ipfname;

	if (fd == -1)
		if ((fd = open(ipfdev, O_RDWR)) == -1)
			if ((fd = open(ipfdev, O_RDONLY)) == -1)
				perror("open device");

	if (setzone(fd) != 0) {
		close(fd);
		return -2;
	}

	return fd;
}


static void closedevice()
{
	close(fd);
	fd = -1;
}


static	int	get_flags()
{
	int i;

	if ((opendevice(ipfname, 1) != -2) &&
	    (ioctl(fd, SIOCGETFF, &i) == -1)) {
		perror("SIOCGETFF");
		return 0;
	}
	return i;
}


static	void	set_state(enable)
u_int	enable;
{
	if (opendevice(ipfname, 0) != -2)
		if (ioctl(fd, SIOCFRENB, &enable) == -1) {
			if (errno == EBUSY)
				fprintf(stderr,
					"IP FIlter: already initialized\n");
			else
				perror("SIOCFRENB");
		}
	return;
}


static	void	procfile(name, file)
char	*name, *file;
{
	(void) opendevice(ipfname, 1);

	initparse();

	ipf_parsefile(fd, ipf_interceptadd, iocfunctions, file);

	if (outputc) {
		printC(0);
		printC(1);
		emit(-1, -1, NULL, NULL);
	}
}


static void ipf_interceptadd(fd, ioctlfunc, ptr)
int fd;
ioctlfunc_t ioctlfunc;
void *ptr;
{
	if (outputc)
		printc(ptr);

	ipf_addrule(fd, ioctlfunc, ptr);
}


static void packetlogon(opt)
char	*opt;
{
	int	flag, xfd, logopt, change = 0;

	flag = get_flags();
	if (flag != 0) {
		if ((opts & (OPT_DONOTHING|OPT_VERBOSE)) == OPT_VERBOSE)
			printf("log flag is currently %#x\n", flag);
	}

	flag &= ~(FF_LOGPASS|FF_LOGNOMATCH|FF_LOGBLOCK);

	if (strstr(opt, "pass")) {
		flag |= FF_LOGPASS;
		if (opts & OPT_VERBOSE)
			printf("set log flag: pass\n");
		change = 1;
	}
	if (strstr(opt, "nomatch")) {
		flag |= FF_LOGNOMATCH;
		if (opts & OPT_VERBOSE)
			printf("set log flag: nomatch\n");
		change = 1;
	}
	if (strstr(opt, "block") || index(opt, 'd')) {
		flag |= FF_LOGBLOCK;
		if (opts & OPT_VERBOSE)
			printf("set log flag: block\n");
		change = 1;
	}
	if (strstr(opt, "none")) {
		if (opts & OPT_VERBOSE)
			printf("disable all log flags\n");
		change = 1;
	}

	if (change == 1) {
		if (opendevice(ipfname, 1) != -2 &&
		    (ioctl(fd, SIOCSETFF, &flag) != 0))
			perror("ioctl(SIOCSETFF)");
	}

	if ((opts & (OPT_DONOTHING|OPT_VERBOSE)) == OPT_VERBOSE) {
		flag = get_flags();
		printf("log flags are now %#x\n", flag);
	}

	if (strstr(opt, "state")) {
		if (opts & OPT_VERBOSE)
			printf("set state log flag\n");
		xfd = open(IPSTATE_NAME, O_RDWR);
		if (xfd >= 0 && setzone(xfd) != 0) {
			close(xfd);
			xfd = -1;
		}

		if (xfd >= 0) {
			logopt = 0;
			if (ioctl(xfd, SIOCGETLG, &logopt))
				perror("ioctl(SIOCGETLG)");
			else {
				logopt = 1 - logopt;
				if (ioctl(xfd, SIOCSETLG, &logopt))
					perror("ioctl(SIOCSETLG)");
			}
			close(xfd);
		}
	}

	if (strstr(opt, "nat")) {
		if (opts & OPT_VERBOSE)
			printf("set nat log flag\n");
		xfd = open(IPNAT_NAME, O_RDWR);
		if (xfd >= 0 && setzone(xfd) != 0) {
			close(xfd);
			xfd = -1;
		}

		if (xfd >= 0) {
			logopt = 0;
			if (ioctl(xfd, SIOCGETLG, &logopt))
				perror("ioctl(SIOCGETLG)");
			else {
				logopt = 1 - logopt;
				if (ioctl(xfd, SIOCSETLG, &logopt))
					perror("ioctl(SIOCSETLG)");
			}
			close(xfd);
		}
	}
}


static	void	flushfilter(arg)
char	*arg;
{
	int	fl = 0, rem;

	if (!arg || !*arg)
		return;
	if (!strcmp(arg, "s") || !strcmp(arg, "S")) {
		if (*arg == 'S')
			fl = FLUSH_TABLE_ALL;
		else
			fl = FLUSH_TABLE_CLOSING;
		rem = fl;

		closedevice();
		if (opendevice(IPSTATE_NAME, 1) == -2)
			exit(1);

		if (!(opts & OPT_DONOTHING)) {
			if (use_inet6) {
				if (ioctl(fd, SIOCIPFL6, &fl) == -1) {
					perror("ioctl(SIOCIPFL6)");
					exit(1);
				}
			} else {
				if (ioctl(fd, SIOCIPFFL, &fl) == -1) {
					perror("ioctl(SIOCIPFFL)");
					exit(1);
				}
			}
		}
		if ((opts & (OPT_DONOTHING|OPT_VERBOSE)) == OPT_VERBOSE) {
			printf("remove flags %s (%d)\n", arg, rem);
			printf("removed %d filter rules\n", fl);
		}
		closedevice();
		return;
	}

#ifdef	SIOCIPFFA
	if (!strcmp(arg, "u")) {
		closedevice();
		/*
		 * Flush auth rules and packets
		 */
		if (opendevice(IPL_AUTH, 1) == -1)
			perror("open(IPL_AUTH)");
		else {
			if (ioctl(fd, SIOCIPFFA, &fl) == -1)
				perror("ioctl(SIOCIPFFA)");
		}
		closedevice();
		return;
	}
#endif

	if (strchr(arg, 'i') || strchr(arg, 'I'))
		fl = FR_INQUE;
	if (strchr(arg, 'o') || strchr(arg, 'O'))
		fl = FR_OUTQUE;
	if (strchr(arg, 'a') || strchr(arg, 'A'))
		fl = FR_OUTQUE|FR_INQUE;
	if (opts & OPT_INACTIVE)
		fl |= FR_INACTIVE;
	rem = fl;

	if (opendevice(ipfname, 1) == -2)
		exit(1);

	if (!(opts & OPT_DONOTHING)) {
		if (use_inet6) {
			if (ioctl(fd, SIOCIPFL6, &fl) == -1) {
				perror("ioctl(SIOCIPFL6)");
				exit(1);
			}
		} else {
			if (ioctl(fd, SIOCIPFFL, &fl) == -1) {
				perror("ioctl(SIOCIPFFL)");
				exit(1);
			}
		}
	}

	if ((opts & (OPT_DONOTHING|OPT_VERBOSE)) == OPT_VERBOSE) {
		printf("remove flags %s%s (%d)\n", (rem & FR_INQUE) ? "I" : "",
			(rem & FR_OUTQUE) ? "O" : "", rem);
		printf("removed %d filter rules\n", fl);
	}
	return;
}


static void swapactive()
{
	int in = 2;

	if (opendevice(ipfname, 1) != -2 && ioctl(fd, SIOCSWAPA, &in) == -1)
		perror("ioctl(SIOCSWAPA)");
	else
		printf("Set %d now inactive\n", in);
}


void ipf_frsync()
{
	int frsyn = 0;

	if (opendevice(ipfname, 1) != -2 && ioctl(fd, SIOCFRSYN, &frsyn) == -1)
		perror("SIOCFRSYN");
	else
		printf("filter sync'd\n");
}


void zerostats()
{
	friostat_t	fio;
	friostat_t	*fiop = &fio;

	if (opendevice(ipfname, 1) != -2) {
		if (ioctl(fd, SIOCFRZST, &fiop) == -1) {
			perror("ioctl(SIOCFRZST)");
			exit(-1);
		}
		showstats(fiop);
	}

}


/*
 * read the kernel stats for packets blocked and passed
 */
static void showstats(fp)
friostat_t	*fp;
{
	printf("bad packets:\t\tin %lu\tout %lu\n",
			fp->f_st[0].fr_bad, fp->f_st[1].fr_bad);
	printf(" input packets:\t\tblocked %lu passed %lu nomatch %lu",
			fp->f_st[0].fr_block, fp->f_st[0].fr_pass,
			fp->f_st[0].fr_nom);
	printf(" counted %lu\n", fp->f_st[0].fr_acct);
	printf("output packets:\t\tblocked %lu passed %lu nomatch %lu",
			fp->f_st[1].fr_block, fp->f_st[1].fr_pass,
			fp->f_st[1].fr_nom);
	printf(" counted %lu\n", fp->f_st[0].fr_acct);
	printf(" input packets logged:\tblocked %lu passed %lu\n",
			fp->f_st[0].fr_bpkl, fp->f_st[0].fr_ppkl);
	printf("output packets logged:\tblocked %lu passed %lu\n",
			fp->f_st[1].fr_bpkl, fp->f_st[1].fr_ppkl);
	printf(" packets logged:\tinput %lu-%lu output %lu-%lu\n",
			fp->f_st[0].fr_pkl, fp->f_st[0].fr_skip,
			fp->f_st[1].fr_pkl, fp->f_st[1].fr_skip);
}


static int showversion()
{
	struct friostat fio;
	ipfobj_t ipfo;
	u_32_t flags;
	char *s;
	int vfd;

	bzero((caddr_t)&ipfo, sizeof(ipfo));
	ipfo.ipfo_rev = IPFILTER_VERSION;
	ipfo.ipfo_size = sizeof(fio);
	ipfo.ipfo_ptr = (void *)&fio;
	ipfo.ipfo_type = IPFOBJ_IPFSTAT;

	printf("ipf: %s (%d)\n", IPL_VERSION, (int)sizeof(frentry_t));

	if ((vfd = open(ipfname, O_RDONLY)) == -1) {
		perror("open device");
		return 1;
	}

	if (setzone(vfd) != 0) {
		close(vfd);
		return 1;
	}

	if (ioctl(vfd, SIOCGETFS, &ipfo)) {
		perror("ioctl(SIOCGETFS)");
		close(vfd);
		return 1;
	}
	close(vfd);
	flags = get_flags();

	printf("Kernel: %-*.*s\n", (int)sizeof(fio.f_version),
		(int)sizeof(fio.f_version), fio.f_version);
	printf("Running: %s\n", (fio.f_running > 0) ? "yes" : "no");
	printf("Log Flags: %#x = ", flags);
	s = "";
	if (flags & FF_LOGPASS) {
		printf("pass");
		s = ", ";
	}
	if (flags & FF_LOGBLOCK) {
		printf("%sblock", s);
		s = ", ";
	}
	if (flags & FF_LOGNOMATCH) {
		printf("%snomatch", s);
		s = ", ";
	}
	if (flags & FF_BLOCKNONIP) {
		printf("%snonip", s);
		s = ", ";
	}
	if (!*s)
		printf("none set");
	putchar('\n');

	printf("Default: ");
	if (FR_ISPASS(fio.f_defpass))
		s = "pass";
	else if (FR_ISBLOCK(fio.f_defpass))
		s = "block";
	else
		s = "nomatch -> block";
	printf("%s all, Logging: %savailable\n", s, fio.f_logging ? "" : "un");
	printf("Active list: %d\n", fio.f_active);
	printf("Feature mask: %#x\n", fio.f_features);

	return 0;
}
