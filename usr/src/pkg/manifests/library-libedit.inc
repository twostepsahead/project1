# Copyright 2017 Lauri Tirkkonen <lotheac@iki.fi>
#
# Permission to use, copy, modify, and/or distribute this software for any
# purpose with or without fee is hereby granted, provided that the above
# copyright notice and this permission notice appear in all copies.
#
# THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
# WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
# MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
# ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
# WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
# ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
# OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
#
dir path=usr group=sys
dir path=usr/include
dir path=usr/lib
dir path=usr/lib/i386
dir path=usr/share
dir path=usr/share/man
dir path=usr/share/man/man3
dir path=usr/share/man/man5
dir path=usr/share/man/man7
file path=usr/include/histedit.h
file path=usr/lib/i386/libedit.so.5.2 mode=0555
file path=usr/lib/libedit.so.5.2 mode=0555
file path=usr/share/man/man3/editline.3
file path=usr/share/man/man5/editrc.5
file path=usr/share/man/man7/editline.7
link path=usr/lib/i386/libedit.so target=libedit.so.5.2
link path=usr/lib/i386/libedit.so.5 target=libedit.so.5.2
link path=usr/lib/libedit.so target=libedit.so.5.2
link path=usr/lib/libedit.so.5 target=libedit.so.5.2
