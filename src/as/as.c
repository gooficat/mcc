#include "as/as.h"
#include "as/as_assem.h"
#include "as/as_tk.h"
#include "as/expr_gen.h"

void as(str path)
{
   fstream_t fs;
   fsopen(fs, path, FMODE_RT);

   as_tok_block block;
   block.toks = as_fetch_tokens(addr fs);
   block.i = 0;

   as_unencoded_unit unencoded;
   init_arr(unencoded.exprs);
   init_arr(unencoded.labels);

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
         grow_arr(unencoded.exprs, 1);
         unencoded.exprs.data[unencoded.exprs.length - 1] = expr;
      }
      else
      {
         ++block.i;
      }
   }

   for (u64 i = 0; i != unencoded.exprs.length; ++i)
   {
      fancc(RED, "\ninstr: %s, args: %hhu", unencoded.exprs.data[i].mnemonic,
            unencoded.exprs.data[i].argc);
      for (u8 j = 0; j != unencoded.exprs.data[i].argc; ++j)
      {
         fancc(PURPLE, "\n\targ: %hhu %llu %hhu", unencoded.exprs.data[i].args[j].base,
               unencoded.exprs.data[i].args[j].disp, unencoded.exprs.data[i].args[j].index);
      }
   }
   printf("\n\n");

   u8_arr bytes = as_encode(addr unencoded);

   for (u64 i = 0; i != bytes.length; ++i)
   {
      fancc(BLUE, "%02hhx\t", bytes.data[i]);
   }

   fsclose(fs);
}
