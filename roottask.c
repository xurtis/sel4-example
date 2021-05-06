/*
 * Copyright 2021, Data61, CSIRO (ABN 41 687 119 230)
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include "debug.h"

int main(void)
{
    /* Space after all the boot text on the serial output */
    debug_puts("\n\n\n");

    /* Print a fancy header */
    debug_puts("Example App\n");
    debug_puts("===========\n\n");

    /* Classic hello world */
    debug_puts("Hello, world!\n");

    /* Leave some space at the end before serial output */
    debug_puts("\n\n\n");
}
