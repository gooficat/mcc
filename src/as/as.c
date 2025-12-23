#include "as/as.h"
#include "as/as_tk.h"
#include "as/expr_gen.h"

void as(str path)
{
   fstream_t fs;
   fsopen(fs, path, FMODE_RT);

   as_tok_block block;
   block.toks = as_fetch_tokens(addr fs);
   block.i = 0;

   while (block.i < block.toks.length)
   {
      fancc(PURPLE, "\n%c", block.toks.data[block.i].type);
      if (block.toks.data[block.i].value notis NULL)
      {
         fancc(BLUE, ": %s", block.toks.data[block.i].value);
      }

      if (block.toks.data[block.i].type is AS_KEW and
          block.toks.data[block.i + 1].type notis AS_LAB)
      {
         as_expr expr = gen_expr(addr block);

         fancc(RED, "\ninstr: %s", expr.mnemonic);
      }
      else
      {
         ++block.i;
      }
   }

   fsclose(fs);
}
