

#include "interop.h"
#include <stdio.h>
#include <stdlib.h>

#define SIZE 1024


void testfunc(MEMREF_AS_ARGS_DEF(my, i32, 2));

void main() {
    
    struct i32_memref_r_2 memref = { 0, 0, 0, SIZE, SIZE+1, 1, 1 };

    memref.allocated = aligned_alloc(64, SIZE * (SIZE + 1) * sizeof(i32));
    memref.aligned   = memref.allocated;

    printf("memref<%ldx%ldxi32>\n", memref.sizes[0], memref.sizes[1]);
    i64 q = 7919;
    // put some random stuff in there
    for (i64 i = 0; i < memref.sizes[0]; i++) {
        for (i64 j = 0; j < memref.sizes[1]; j++) {
            LIN_ACCESS2(memref, i, j) = (i + j) % ('A' - 'Z' + 1) + 'A';
        }
    }

    testfunc(MEMREF_TO_ARGS(memref, 2));

    free(memref.allocated);
}

void testfunc(MEMREF_AS_ARGS_DEF(my, i32, 2)) {
    COLLECT_MEMREF_ARGS_INTO(my, i32, 2, my_memref);

    DUMP_MEMREF("outfile", my_memref, i32, 2)
}