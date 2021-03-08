/*
 * Copyright 2021, Data61, CSIRO (ABN 41 687 119 230)
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <sel4/sel4.h>
#include <sel4runtime.h>

static void debug_puts(const char *text)
{
    while (text && *text != '\0') {
        seL4_DebugPutChar(*text);
        text++;
    }
}

/* Print an integer */
static void debug_puti(int i)
{
    unsigned n = 1;

    if (i < 0) {
        seL4_DebugPutChar('-');
        i *= -1;
    }

    while (n < i) n *= 10;

    while (n > 1) {
        seL4_DebugPutChar('0' + i / n);
        i -= i / n;
        n /= 10;
    }

    seL4_DebugPutChar('0' + i);
}

/* Exit by suspending the thread */
static void exit_callback(int code)
{
    debug_puts("Exiting with exit code #");
    debug_puti(code);
    debug_puts("\n");

    seL4_TCB_Suspend(seL4_CapInitThreadTCB);
}

int main(void)
{
    /* Set exit handler */
    sel4runtime_set_exit(exit_callback);

    debug_puts("Health check passed\n");
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

/* This needs to handle assertion failures in libsel4 */
void __assert_fail (
    const char *assertion,
    const char *file,
    int line,
    const char *function
) {
    debug_puts("Assertion failed: (");
    debug_puts(assertion);
    debug_puts(") of function ");
    debug_puts(function);
    debug_puts(" at ");
    debug_puts(file);
    debug_puts(":");
    debug_puti(line);
    debug_puts(".\n");
    sel4runtime_exit(-1);
}
