/*
 * Copyright 2017 Lauri Tirkkonen <lotheac@iki.fi>
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#include <err.h>
#include <stdlib.h>
#include <unistd.h>

static void
usage(void)
{
	printf("usage: puname command [args]\n");
	exit(1);
}

int
main(int argc, char **argv)
{
	if (argc < 2)
		usage();

	if (setenv("UNAME_LEGACY", "", 0) < 0)
		err(1, "setenv");
	argv++;
	execvp(argv[0], argv);
	err(1, "cannot execute %s", argv[0]);
	return 1;
}
