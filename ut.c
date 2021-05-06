/*
 * Copyright 2021, Data61, CSIRO (ABN 41 687 119 230)
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <sel4runtime.h>
#include "ut.h"
#include "fail.h"

/* CPtr used for temporary storage of untypeds */
static seL4_CPtr scratch_cptr;

/* Caps to existing untypeds of each size up to 2^16 */
static seL4_CPtr existing_uts[MAX_UT_SIZE];

/* Index in the bootinfo untypedList to current UT to create 2^16 byte
 * UTs. */
static unsigned master_ut_index;

/* Number of 2^16 byte UTs created from the current master */
static unsigned master_uts_created;

/* Allocate a new 64KiB untyped */
static seL4_CPtr new_64k_ut(void);

__attribute__((constructor(UT_INIT_PRIO)))
static void init_ut(void)
{
    /* Allocate a scratch CPtr */
    scratch_cptr = alloc_cptr();
}

seL4_CPtr ut_alloc(unsigned size_bits)
{
    assert(size_bits < MAX_UT_SIZE);

    if (existing_uts[size_bits] != seL4_CapNull) {
        /* Already have a UT of the appropriate size */
        seL4_CPtr ut = existing_uts[size_bits];
        existing_uts[size_bits] = seL4_CapNull;
        return ut;
    }

    /* Break apart a larger size */
    int larger_size_bits = size_bits + 1;
    while (
        existing_uts[larger_size_bits] == seL4_CapNull &&
        larger_size_bits < MAX_UT_SIZE
    ) {
        larger_size_bits += 1;
    }

    seL4_CPtr larger;
    if (larger_size_bits < MAX_UT_SIZE) {
        larger = existing_uts[larger_size_bits];
        existing_uts[larger_size_bits] = seL4_CapNull;
    } else {
        larger = new_64k_ut();
    }

    assert(larger != seL4_CapNull);

    /* Create the smaller UTs */
    while (larger_size_bits > size_bits) {
        larger_size_bits -= 1;

        /* Create the first of two copies that will be kept for later */
        existing_uts[larger_size_bits] = alloc_cptr();
        seL4_Error err = seL4_Untyped_Retype(
                             larger,
                             seL4_UntypedObject,
                             larger_size_bits,
                             seL4_CapInitThreadCNode,
                             0,
                             0,
                             existing_uts[larger_size_bits],
                             1
                         );
        assert(err == seL4_NoError);

        /* Create the second copy that we will use for the requested
         * allocation */
        err = seL4_Untyped_Retype(
                  larger,
                  seL4_UntypedObject,
                  larger_size_bits,
                  seL4_CapInitThreadCNode,
                  0,
                  0,
                  scratch_cptr,
                  1
              );
        assert(err == seL4_NoError);

        /* Delete the exhausted larger untyped */
        err = seL4_CNode_Delete(
                  seL4_CapInitThreadCNode,
                  larger,
                  seL4_WordBits
              );
        assert(err == seL4_NoError);

        /* Move the used copy into the larger slot from the scratch */
        err = seL4_CNode_Move(
                  seL4_CapInitThreadCNode,
                  larger,
                  seL4_WordBits,
                  seL4_CapInitThreadCNode,
                  scratch_cptr,
                  seL4_WordBits
              );
        assert(err == seL4_NoError);
    }

    return larger;
}

seL4_CPtr kobj_alloc(unsigned size_bits, seL4_ObjectType type)
{
    seL4_CPtr cptr = ut_alloc(size_bits);

    /* Create the object */
    seL4_Error err = seL4_Untyped_Retype(
                         cptr,
                         type,
                         size_bits,
                         seL4_CapInitThreadCNode,
                         0,
                         0,
                         scratch_cptr,
                         1
                     );
    assert(err == seL4_NoError);

    /* Delete the untyped */
    err = seL4_CNode_Delete(
              seL4_CapInitThreadCNode,
              cptr,
              seL4_WordBits
          );
    assert(err == seL4_NoError);

    /* Move the object from the scratch slot into the slot from the
     * untyped */
    err = seL4_CNode_Move(
              seL4_CapInitThreadCNode,
              cptr,
              seL4_WordBits,
              seL4_CapInitThreadCNode,
              scratch_cptr,
              seL4_WordBits
          );
    assert(err == seL4_NoError);

    return cptr;
}

static seL4_CPtr new_64k_ut(void)
{
    seL4_SlotRegion untyped = sel4runtime_bootinfo()->untyped;
    seL4_UntypedDesc *descs = sel4runtime_bootinfo()->untypedList;
    while (master_ut_index < untyped.end - untyped.start) {
        seL4_Uint8 ut_size_bits = descs[master_ut_index].sizeBits;
        unsigned master_uts = 1 << (ut_size_bits - (MAX_UT_SIZE - 1));
        if (
            !descs[master_ut_index].isDevice &&
            ut_size_bits <= MAX_UT_SIZE - 1 &&
            master_uts_created < master_uts
        ) {
            /* Create a new large untyped */
            seL4_CPtr cptr = alloc_cptr();
            seL4_Error err = seL4_Untyped_Retype(
                                 untyped.start + master_ut_index,
                                 seL4_UntypedObject,
                                 MAX_UT_SIZE - 1,
                                 seL4_CapInitThreadCNode,
                                 0,
                                 0,
                                 cptr,
                                 1
                             );
            assert(err == seL4_NoError);

            master_uts_created += 1;
            return cptr;
        }

        master_ut_index += 1;
        master_uts_created = 0;
    }

    return seL4_CapNull;
}
