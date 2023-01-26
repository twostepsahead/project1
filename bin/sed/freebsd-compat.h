#ifndef _FREEBSD_COMPAT_H
#define _FREEBSD_COMPAT_H
#define ALLPERMS	(S_ISUID|S_ISGID|S_IRWXU|S_IRWXG|S_IRWXO)
#define DEFFILEMODE	(S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP|S_IROTH|S_IWOTH)
#endif /* _FREEBSD_COMPAT_H */