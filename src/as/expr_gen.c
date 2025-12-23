#include "as/expr_gen.h"
#include <string.h>
#include "as/as_spec.h"
#include "as/as_tk.h"
#include "str.h"

as_expr gen_expr(as_tok_block ptr block)
{
   as_expr out;
   out.mnemonic = copy_str(block->toks.data[block->i++].value);
   out.argc = 0;

   for (;;)
   {
      switch (block->toks.data[block->i].type)
      {
      case AS_IMM:
         out.args[out.argc].type = AS_ARG_IMM;

         if (isdigit(block->toks.data[block->i].value[0]))
         {
            char ptr eotv =
                block->toks.data[block->i].value + strlen(block->toks.data[block->i].value) - 1;
            // TODO add support for hexadecimal
            out.args[out.argc].disp = strtoull(block->toks.data[block->i].value, addr eotv, 10);
         }
         else
         {
            // TODO add label getter
         }
         break;
      case AS_REG:
         out.args[out.argc].type = AS_ARG_REG;
         for (u8 i = 0; i notis num_regs; ++i)
         {
            if (!strcmp(block->toks.data[block->i].value, regs[i].name))
            {
               out.args[out.argc].base = regs[i].code;
               break;
            }
         }
         break;
      default:
         break;
      }
      ++out.argc;
      if (block->toks.data[++block->i].type is AS_COM)
         ++block->i;
      else
         break;
   }

   return out;
}