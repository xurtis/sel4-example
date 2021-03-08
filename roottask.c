/*
 * Copyright 2021, Data61, CSIRO (ABN 41 687 119 230)
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <sel4/sel4.h>

static void debug_puts(char *text)
{
    while (text && *text != '\0') {
        seL4_DebugPutChar(*text);
        text++;
    }
}

int main(void)
{
    debug_puts("Health check passed\n");
}
