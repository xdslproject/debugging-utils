/*
Utility file to facilitate debugging of an .mlir file
See example at the end of the file
*/

#include <stdio.h>

void print_time(long unsigned i) {
    printf("time: %lu\n", i);
}
void print_float(float i) {
    printf("element value: %.2f\n", i);
}
void print_idx_3(float i, long unsigned t, long unsigned x, long unsigned y) {
    printf("u[%lu][%lu][%lu] = %.2f\n", t, x, y, i);
}


/*
Add these manually in the generated `.iet.mlir` file, at the bottom into the "builtin.module"

llvm.func @print_time(i32) -> () attributes {sym_visibility = "private"}
llvm.func @print_float(f32) -> () attributes {sym_visibility = "private"}
llvm.func @print_idx_3(f32, i32, i32, i32) -> () attributes {sym_visibility = "private"}

Then you can call them using:

llvm.call @print_time(%time) : (i32) -> ()
llvm.call @print_float(%val) : (f32) -> ()
llvm.call @print_idx_3(%val, %t, %x, %y) : (f32, i32, i32, i32) -> ()

*/
