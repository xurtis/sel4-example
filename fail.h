/*
 * Copyright 2021, Data61, CSIRO (ABN 41 687 119 230)
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#define _assert_str(_e) (#_e)
#define assert(_e) \
    if (!(_e)) {\
        __assert_fail(_assert_str(_e), __FILE__, __LINE__, __func__);\
    }

/* Print out an assertion failure */
void __assert_fail(
    const char *assertion,
    const char *file,
    int line,
    const char *function
);
