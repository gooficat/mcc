#pragma once
#include "snips.h"

#define TK_MAX 32

enu_m{
    AS_EOF, AS_REG, AS_KEW, AS_IMM, AS_MEM, AS_ADD, AS_SUB, AS_MUL,
} as_toktype;

struc_t
{
   as_toktype type;
   char* value;
}
as_tok;
struc_t array_struct(as_tok) as_tok_arr;

as_tok_arr as_fetch_tokens(fstream_t ptr fs);
