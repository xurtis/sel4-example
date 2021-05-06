/*
 * Copyright 2021, Data61, CSIRO (ABN 41 687 119 230)
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <sel4/sel4.h>
#include <sel4runtime.h>
#include "debug.h"

/* Exit callback */
static void exit_callback(int code);

/*
 * The runtime does not encode any exit behaviour as that is entirely
 * dependent on the environment. We use a constructor here, which is run
 * before main, to set the callback that the runtime will call to handle
 * an exit.
 */
__attribute__((constructor))
static void set_exit_callback(void)
{
    sel4runtime_set_exit(exit_callback);
}

/*
 * Our exit callback will display the status code provided at exit,
 * which may indicate a different exit for an error, and will suspend
 * the initial thread.
 */
static void exit_callback(int code)
{
    debug_puts("\n\n");
    debug_puts("Exiting with exit code #");
    debug_puti(code);
    debug_puts("\n\n\n");

    seL4_TCB_Suspend(seL4_CapInitThreadTCB);
}
