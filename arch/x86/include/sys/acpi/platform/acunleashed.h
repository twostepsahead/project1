#ifndef _ACUNLEASHED_H_
#define	_ACUNLEASHED_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <sys/types.h>
#include <sys/sunddi.h>
#include <sys/varargs.h>
#include <sys/cpu.h>
#include <sys/thread.h>

#ifdef _KERNEL
#include <sys/ctype.h>
#else
#include <ctype.h>
#include <strings.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#endif

/* Function name used for debug output. */
#define	ACPI_GET_FUNCTION_NAME	__func__

uint32_t __acpi_acquire_global_lock(void *);
uint32_t __acpi_release_global_lock(void *);
void	 __acpi_wbinvd(void);
uint32_t acpi_strtoul(const char *, char **, int);

#ifdef	_ILP32
#define	ACPI_MACHINE_WIDTH	32
#define	COMPILER_DEPENDENT_INT64	int64_t
#define	COMPILER_DEPENDENT_UINT64	uint64_t

#elif	defined(_LP64)
#define	ACPI_MACHINE_WIDTH	64
#define	COMPILER_DEPENDENT_INT64	ssize_t
#define	COMPILER_DEPENDENT_UINT64	size_t

#endif

#define	ACPI_CAST_PTHREAD_T(pthread)	((ACPI_THREAD_ID) (pthread))

#define	ACPI_USE_NATIVE_DIVIDE
#define	ACPI_FLUSH_CPU_CACHE()	(__acpi_wbinvd())

#ifndef ACPI_DISASSEMBLER
#define	ACPI_DISASSEMBLER
#endif
#define	ACPI_PACKED_POINTERS_NOT_SUPPORTED

/*
 * Calling conventions:
 *
 * ACPI_SYSTEM_XFACE        - Interfaces to host OS (handlers, threads)
 * ACPI_EXTERNAL_XFACE      - External ACPI interfaces
 * ACPI_INTERNAL_XFACE      - Internal ACPI interfaces
 * ACPI_INTERNAL_VAR_XFACE  - Internal variable-parameter list interfaces
 */
#define	ACPI_SYSTEM_XFACE
#define	ACPI_EXTERNAL_XFACE
#define	ACPI_INTERNAL_XFACE
#define	ACPI_INTERNAL_VAR_XFACE

/*
 * The ACPI headers shipped from Intel defines a bunch of functions which are
 * already provided by the kernel.  The variable below prevents those from
 * being loaded as part of accommon.h.
 */
#define	ACPI_USE_SYSTEM_CLIBRARY

#ifdef _KERNEL
#define	strtoul(s, r, b)	acpi_strtoul(s, r, b)
#define	toupper(x)		(islower(x) ? (x) - 'a' + 'A' : (x))
#define	tolower(x)		(isupper(x) ? (x) - 'A' + 'a' : (x))
#endif

#define	ACPI_ASM_MACROS
#define	BREAKPOINT3
#define	ACPI_DISABLE_IRQS()	cli()
#define	ACPI_ENABLE_IRQS()	sti()
#define	ACPI_ACQUIRE_GLOBAL_LOCK(Facs, Acq)	\
	((Acq) = __acpi_acquire_global_lock(Facs))

#define	ACPI_RELEASE_GLOBAL_LOCK(Facs, Acq)	\
	((Acq) = __acpi_release_global_lock(Facs))

#ifdef __cplusplus
}
#endif

#endif /* ACUNLEASHED_H_ */
