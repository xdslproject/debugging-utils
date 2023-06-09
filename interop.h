#pragma once
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

typedef float f32;
typedef double f64;

typedef int32_t i32;
typedef int64_t i64;

#define MEMREF_STRUCT_DEF(dtype, rank) struct dtype ## _memref_r_ ## rank {    \
  dtype *allocated;                          \
  dtype *aligned;                            \
  intptr_t offset;                           \
  intptr_t sizes[rank];                      \
  intptr_t strides[rank];                    \
};                                           \

#define OUTFILE_NAME "outfile"

// define memref rank 1 to 3 for f32, f64, i32, i64

MEMREF_STRUCT_DEF(f32, 1)
MEMREF_STRUCT_DEF(f32, 2)
MEMREF_STRUCT_DEF(f32, 3)

MEMREF_STRUCT_DEF(f64, 1)
MEMREF_STRUCT_DEF(f64, 2)
MEMREF_STRUCT_DEF(f64, 3)

MEMREF_STRUCT_DEF(i32, 1)
MEMREF_STRUCT_DEF(i32, 2)
MEMREF_STRUCT_DEF(i32, 3)

MEMREF_STRUCT_DEF(i64, 1)
MEMREF_STRUCT_DEF(i64, 2)
MEMREF_STRUCT_DEF(i64, 3)


// code for packing/unpacking memrefs to/from args
// please don't look at this too closely here:
#define REP0(X)
#define REP1(X) X ## _1
#define REP2(X) REP1(X) , X ## _2
#define REP3(X) REP2(X) , X ## _3
#define REP4(X) REP3(X) , X ## _4
#define REP5(X) REP4(X) , X ## _5
#define REP6(X) REP5(X) , X ## _6
#define REP7(X) REP6(X) , X ## _7
#define REP8(X) REP7(X) , X ## _8
#define REP9(X) REP8(X) , X ## _9
#define REP10(X) REP9(X) , X ## _10


#define UNPACK_REP0(X)
#define UNPACK_REP1(X) X[0]
#define UNPACK_REP2(X) UNPACK_REP1(X) , X[1]
#define UNPACK_REP3(X) UNPACK_REP2(X) , X[2]
#define UNPACK_REP4(X) UNPACK_REP3(X) , X[3]
#define UNPACK_REP5(X) UNPACK_REP4(X) , X[4]
#define UNPACK_REP6(X) UNPACK_REP5(X) , X[5]
#define UNPACK_REP7(X) UNPACK_REP6(X) , X[6]
#define UNPACK_REP8(X) UNPACK_REP7(X) , X[7]
#define UNPACK_REP9(X) UNPACK_REP8(X) , X[8]
#define UNPACK_REP10(X) UNPACK_REP9(X) , X[9]

#define UNPACK_NO_COMMA_REP0(X)
#define UNPACK_NO_COMMA_REP1(X) X[0]
#define UNPACK_NO_COMMA_REP2(X) UNPACK_NO_COMMA_REP1(X) X[1]
#define UNPACK_NO_COMMA_REP3(X) UNPACK_NO_COMMA_REP2(X) X[2]
#define UNPACK_NO_COMMA_REP4(X) UNPACK_NO_COMMA_REP3(X) X[3]
#define UNPACK_NO_COMMA_REP5(X) UNPACK_NO_COMMA_REP4(X) X[4]
#define UNPACK_NO_COMMA_REP6(X) UNPACK_NO_COMMA_REP5(X) X[5]
#define UNPACK_NO_COMMA_REP7(X) UNPACK_NO_COMMA_REP6(X) X[6]
#define UNPACK_NO_COMMA_REP8(X) UNPACK_NO_COMMA_REP7(X) X[7]
#define UNPACK_NO_COMMA_REP9(X) UNPACK_NO_COMMA_REP8(X) X[8]
#define UNPACK_NO_COMMA_REP10(X) UNPACK_NO_COMMA_REP9(X) X[9]

// oh god, this is unholy:

#define MEMREF_AS_ARGS_DEF(prefix, dtype, rank) dtype * prefix ## allocated, dtype * prefix ## aligned, intptr_t prefix ## offset, REP ## rank (intptr_t prefix ## sizes), REP ## rank (intptr_t prefix ## strides) 

#define COLLECT_MEMREF_ARGS_INTO(prefix, dtype, rank, name) struct dtype ## _memref_r_ ## rank name = { prefix ## allocated, prefix ## aligned, prefix ## offset, REP ## rank (prefix ## sizes), REP ## rank (prefix ## strides) }

#define MEMREF_TO_ARGS(ref, rank) ref.allocated, ref.aligned, ref.offset, UNPACK_REP ## rank (ref.sizes), UNPACK_REP ## rank (ref.strides)

// dumping memref macros:

#define DUMP_MEMREF(fname, name, dtype, rank) {\
    FILE* f = fopen(fname,"wb");\
    fwrite(name.aligned, sizeof(dtype), 1 UNPACK_NO_COMMA_REP ## rank(* name.sizes), f);\
    fclose(f);\
}

// linearized accesses:

#define LIN_ACCESS2(ref, x, y) ref.aligned[(x) * ref.sizes[1] + (y)]
#define LIN_ACCESS3(ref, x, y, z) ref.aligned[(x) * ref.sizes[1] * ref.sizes[2] + (y) * ref.sizes[2] + (z)]

// dumping methods:

#define GENERATE_DUMPING_FUNC(dtype, rank) void dump_memref_ ## dtype ## _rank_ ## rank (MEMREF_AS_ARGS_DEF(my, dtype, rank)) { \
    COLLECT_MEMREF_ARGS_INTO(my, dtype, rank, my_memref); \
    DUMP_MEMREF(OUTFILE_NAME, my_memref, dtype, rank) \
}

// generate function defs:

GENERATE_DUMPING_FUNC(f32, 1)
GENERATE_DUMPING_FUNC(f32, 2)
GENERATE_DUMPING_FUNC(f32, 3)

GENERATE_DUMPING_FUNC(f64, 1)
GENERATE_DUMPING_FUNC(f64, 2)
GENERATE_DUMPING_FUNC(f64, 3)

GENERATE_DUMPING_FUNC(i32, 1)
GENERATE_DUMPING_FUNC(i32, 2)
GENERATE_DUMPING_FUNC(i32, 3)

GENERATE_DUMPING_FUNC(i64, 1)
GENERATE_DUMPING_FUNC(i64, 2)
GENERATE_DUMPING_FUNC(i64, 3)

/*
This file provides the following functions for MLIR:

llvm.func @dump_memref_i32_rank_1(!memref<?xi32>) -> () attributes {sym_visibility = "private"}
llvm.func @dump_memref_f32_rank_1(!memref<?xf32>) -> () attributes {sym_visibility = "private"}
llvm.func @dump_memref_i64_rank_1(!memref<?xi64>) -> () attributes {sym_visibility = "private"}
llvm.func @dump_memref_f64_rank_1(!memref<?xf64>) -> () attributes {sym_visibility = "private"}

llvm.func @dump_memref_i32_rank_2(!memref<?x?xi32>) -> () attributes {sym_visibility = "private"}
llvm.func @dump_memref_f32_rank_2(!memref<?x?xf32>) -> () attributes {sym_visibility = "private"}
llvm.func @dump_memref_i64_rank_2(!memref<?x?xi64>) -> () attributes {sym_visibility = "private"}
llvm.func @dump_memref_f64_rank_2(!memref<?x?xf64>) -> () attributes {sym_visibility = "private"}

llvm.func @dump_memref_i32_rank_3(!memref<?x?x?xi32>) -> () attributes {sym_visibility = "private"}
llvm.func @dump_memref_f32_rank_3(!memref<?x?x?xf32>) -> () attributes {sym_visibility = "private"}
llvm.func @dump_memref_i64_rank_3(!memref<?x?x?xi64>) -> () attributes {sym_visibility = "private"}
llvm.func @dump_memref_f64_rank_3(!memref<?x?x?xf64>) -> () attributes {sym_visibility = "private"}

You can call them using:

llvm.call @dump_memref_f64_rank_3(%ref) : (!memref<?x?x?xi64>) -> ()

or any other signature as provided above

The output file will be outfile

*/
