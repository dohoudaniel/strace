/*
 * Check decoding of pipe2 syscall.
 *
 * Copyright (c) 2015-2018 Dmitry V. Levin <ldv@strace.io>
 * Copyright (c) 2017-2024 The strace developers.
 * All rights reserved.
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

#include "tests.h"
#include "scno.h"

#include <stdio.h>
#include <unistd.h>
#include "kernel_fcntl.h"

int
main(void)
{
	TAIL_ALLOC_OBJECT_CONST_ARR(int, fds, 2);
	int *const efault = fds + 1;
	long rc;

	rc = syscall(__NR_pipe2, fds, F8ILL_KULONG_MASK | O_NONBLOCK);
	if (rc)
		perror_msg_and_skip("pipe2");
	printf("pipe2([%d, %d], O_NONBLOCK) = 0\n", fds[0], fds[1]);

	rc = syscall(__NR_pipe2, efault, F8ILL_KULONG_MASK);
	printf("pipe2(%p, 0) = %s\n", efault, sprintrc(rc));

	if (F8ILL_KULONG_SUPPORTED) {
		const kernel_ulong_t ill = f8ill_ptr_to_kulong(fds);
		rc = syscall(__NR_pipe2, ill, 0);
		printf("pipe2(%#llx, 0) = %s\n",
		       (unsigned long long) ill, sprintrc(rc));
	}

	puts("+++ exited with 0 +++");
	return 0;
}
