#include "as/expr_gen.h"
#include "str.h"

as_expr gen_expr(as_tok_block ptr block)
{
   as_expr out;
   out.mnemonic = copy_str(block->toks.data[block->i++].value);

   return out;
}