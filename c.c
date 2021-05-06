/*
 * Copyright 2021, Data61, CSIRO (ABN 41 687 119 230)
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

/*
 * This file provides a minimal subset of the C standard library
 */

#include "c.h"

/* GCC on aarch32 requires memset */
void *__sel4runtime_memset(void *dest, int c, sel4runtime_size_t n);

void *memset(void *s, int c, sel4runtime_size_t n)
{
    // We just steal the memset implementation from the runtime
    return __sel4runtime_memset(s, c, n);
}

/* GCC for aarch32 requires raise to be defined */
int raise(int sig)
{
    // We always exit with the signal number
    sel4runtime_exit(-sig);
    return -sig;
}

/* seL4_DebugNameThread depends on this function being defined */
char *strcpy(char *dest, const char *src)
{
    char *dest_cursor = dest;
    while (*src != '\0') {
        *dest_cursor = *src;
        src++;
        dest_cursor++;
    }
    /* Copy the trailing nul */
    *dest_cursor = *src;

    return dest;
}
