#pragma once
#include "arr.h"
#include "snips.h"

#include "as/expr_gen.h"

struc_t array_struct(as_expr) as_expr_arr;

struc_t
{
   str name;
   u64 index;  // indexed to an expression that it precedes in a unit
}
as_label;

struc_t array_struct(as_label) as_label_arr;

struc_t
{
   as_expr_arr exprs;
   as_label_arr labels;
}
as_unencoded_unit;

struc_t array_struct(u8) u8_arr;

u8_arr as_encode(as_unencoded_unit ptr exprs);