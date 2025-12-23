#pragma once
#include "as/as_tk.h"

#define MAX_ARGS 2

struc_t
{
   as_tok_arr toks;
   u64 i;
}
as_tok_block;

enu_m{
    AS_ARG_IMM,
    AS_ARG_REG,
    AS_ARG_MEM,
} arg_type;

struc_t
{
   arg_type type;
   u8 base;
   u8 index;
   u64 disp;
   char* label_ref;
}
as_arg;

struc_t
{
   str mnemonic;
   as_arg args[MAX_ARGS];
}
as_expr;

as_expr gen_expr(as_tok_block ptr block);