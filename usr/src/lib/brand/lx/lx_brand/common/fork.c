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
 * Copyright 2006 Sun Microsystems, Inc.  All rights reserved.
 * Use is subject to license terms.
 * Copyright 2017 Joyent, Inc.
 */

#include <errno.h>
#include <unistd.h>
#include <sys/fork.h>
#include <sys/syscall.h>
#include <sys/debug.h>
#include <strings.h>
#include <sys/lx_debug.h>
#include <sys/lx_misc.h>
#include <sys/lx_syscall.h>

/*
 * fork() and vfork()
 *
 * These cannot be pass thru system calls because we need libc to do its own
 * initialization or else bad things will happen (i.e. ending up with a bad
 * schedctl page).  On Linux, there is no such thing as forkall(), so we use
 * fork1() here.
 */

long
lx_fork(void)
{
	int ret;

	/*
	 * Inform the in-kernel ptrace(2) subsystem that we are about to
	 * emulate fork(2).
	 */
	lx_ptrace_clone_begin(LX_PTRACE_O_TRACEFORK, B_FALSE, 0);

	/*
	 * Suspend signal delivery, run the stack management prefork handler
	 * and perform the fork operation.
	 */
	_sigoff();
	lx_stack_prefork();
	ret = fork1();
	lx_stack_postfork();

	switch (ret) {
	case -1:
		_sigon();
		return (-errno);

	case 0:
		/*
		 * Returning in the new child.  We must free the stacks and
		 * thread-specific data objects for the threads we did not
		 * duplicate; i.e. every other thread.
		 */
		lx_free_other_stacks();

		lx_ptrace_stop_if_option(LX_PTRACE_O_TRACEFORK, B_TRUE, 0,
		    NULL);

		/*
		 * Re-enable signal delivery in the child and return to the
		 * new process.
		 */
		_sigon();
		return (0);

	default:
		lx_ptrace_stop_if_option(LX_PTRACE_O_TRACEFORK, B_FALSE,
		    (ulong_t)ret, NULL);

		/*
		 * Re-enable signal delivery in the parent and return from
		 * the emulated system call.
		 */
		_sigon();
		return (ret);
	}
}

long
lx_vfork(void)
{
	int ret;
	lx_sighandlers_t saved;
	ucontext_t vforkuc;
	ucontext_t *ucp;
	lx_tsd_t *lx_tsd = lx_get_tsd();

	ucp = lx_syscall_regs();

	/*
	 * Inform the in-kernel ptrace(2) subsystem that we are about to
	 * emulate vfork(2).
	 */
	lx_ptrace_clone_begin(LX_PTRACE_O_TRACEVFORK, B_FALSE, 0);

	/*
	 * Suspend signal delivery, run the stack management prefork handler
	 * and perform the vfork operation. We use the same approach as in
	 * lx_clone for signal handling and child return across vfork. See
	 * the comments in lx_clone for more detail.
	 */

	lx_block_all_signals();
	lx_stack_prefork();
	lx_sighandlers_save(&saved);
	lx_tsd->lxtsd_is_vforked++;
	ret = vfork();
	if (ret != 0) {
		/* parent/error */
		lx_tsd->lxtsd_is_vforked--;
		lx_sighandlers_restore(&saved);
	}

	switch (ret) {
	case -1:
		lx_stack_postfork();
		lx_unblock_all_signals();
		return (-errno);

	case 0:
		/*
		 * child
		 * Unlike the regular fork case where the child also calls
		 * lx_stack_postfork(), we only do that in the parent once it
		 * resumes execution. This is required there to wake any other
		 * threads in that process that are  blocked on the lock taken
		 * in lx_stack_prefork().
		 */
		bcopy(ucp, &vforkuc, sizeof (vforkuc));
		vforkuc.uc_brand_data[1] = (caddr_t)vforkuc.uc_brand_data[1] -
		    LX_NATIVE_STACK_VFORK_GAP;
		vforkuc.uc_link = NULL;

		lx_debug("\tvfork native stack sp %p",
		    vforkuc.uc_brand_data[1]);

		lx_unblock_all_signals();

		/* Stop for ptrace if required. */
		lx_ptrace_stop_if_option(LX_PTRACE_O_TRACEVFORK, B_TRUE, 0,
		    NULL);

		/*
		 * Return to the child via the specially constructed vfork(2)
		 * context.
		 */
		LX_EMULATE_RETURN(&vforkuc, LX_SYS_vfork, 0, 0);
		(void) syscall(SYS_brand, B_EMULATION_DONE, &vforkuc,
		    LX_SYS_vfork, 0, 0);

		VERIFY(0);
		return (0);

	default:
		/* parent - child should have exited or exec-ed by now */
		lx_stack_postfork();
		lx_unblock_all_signals();
		lx_ptrace_stop_if_option(LX_PTRACE_O_TRACEVFORK, B_FALSE,
		    (ulong_t)ret, NULL);
		return (ret);
	}
}
