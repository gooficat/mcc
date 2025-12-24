#include "as/expr_gen.h"
#include <stdint.h>
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
            u64 val = strtoull(block->toks.data[block->i].value, addr eotv, 10);
            byte_size sz;
            if (val > UINT32_MAX)
               sz = sq;
            elif (val > UINT16_MAX) sz = sd;
            elif (val > UINT8_MAX) sz = sw;
            else sz = sb;

            out.args[out.argc].size = sz;
            out.args[out.argc].disp = val;
         }
         else
         {
            // TODO add label getter
         }
         ++out.argc;
         break;
      case AS_REG:
         out.args[out.argc].type = AS_ARG_REG;
         for (u8 i = 0; i notis num_regs; ++i)
         {
            if (not strcmp(block->toks.data[block->i].value, regs[i].name))
            {
               out.args[out.argc].base = regs[i].code;
               out.args[out.argc].size = regs[i].size;
               break;
            }
         }
         ++out.argc;
         break;
      default:
         break;
      }
      if (block->toks.data[++block->i].type is AS_COM)
         ++block->i;
      else
         break;
   }

   return out;
}
