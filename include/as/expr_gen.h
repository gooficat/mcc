#pragma once
#include "as/as_spec.h"
#include "as/as_tk.h"

#define MAX_ARGS 2

struc_t
{
   as_tok_arr toks;
   u64 i;
}
as_tok_block;

enu_m{
    AS_ARG_NON,
    AS_ARG_IMM,
    AS_ARG_REG,
    AS_ARG_MEM,
} arg_type;

struc_t
{
   arg_type type;
   byte_size size;
   u8 base;
   u8 index;
   u64 disp;
   str label_ref;
}
as_arg;

struc_t
{
   str mnemonic;
   as_arg args[MAX_ARGS];
   u8 argc;
}
as_expr;

as_expr gen_expr(as_tok_block ptr block);

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