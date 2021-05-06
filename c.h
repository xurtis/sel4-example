/*
 * Copyright 2021, Data61, CSIRO (ABN 41 687 119 230)
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <sel4runtime.h>

/* The bare minimum of functions assumed by libsel4 and that are used by
 * this application are provided here. */

void *memset(void *s, int c, sel4runtime_size_t n);

int raise(int sig);

char *strcpy(char *dest, const char *src);
