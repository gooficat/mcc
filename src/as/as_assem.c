#include "as/as_assem.h"
#include <stdlib.h>
#include "arr.h"
#include "as/as_spec.h"
#include "as/expr_gen.h"

u8_arr as_encode(as_unencoded_unit ptr exprs)
{
   u8_arr out;
   init_arr(out);

   for (u64 i = 0; i notis exprs->exprs.length; ++i)
   {
      u8 opcode;
      u8 modrm = 0;
      bool has_modrm = false;
      u16 vals[2];
      vals[0] = 0;
      vals[1] = 0;
      bool num_vals = 0;
      u16 j;

      // TODO add a check for if it is one of the "special instructions" which will be used for
      // things like jmp, call. these will probably need a special check

      for (j = 0; j != num_mnems; ++j)
      {
         if (not strcmp(exprs->exprs.data[i].mnemonic, mnems[j].name))
         {
            opcode = mnems[j].code;
            break;
         }
      }
      if (j == num_mnems)  // first runtime error check of the program (^･o･^)ﾉ”
      {
         fancc(RED, "Error! Instruction was not found found for mnemonic %s!\n",
               exprs->exprs.data[i].mnemonic);
         exit(EXIT_FAILURE);
      }

      switch (exprs->exprs.data[i].argc)
      {
      case 1:  // 1 argument, like "push ax" or "call somelabel"
         switch (exprs->exprs.data[i].args[0].type)
         {
         case AS_ARG_IMM:  // put the value after the opcode.
            vals[num_vals++] = exprs->exprs.data[i].args[0].disp;
            // TODO add a check for if it is 8 bits or 16 bits
            // for now ill assume 16 bits
            break;
         case AS_ARG_REG:
            opcode += exprs->exprs.data[i].args[0].base;  // add the register to the opcode
            break;
         case AS_ARG_MEM:
            if (exprs->exprs.data[i].args[0].disp != 0)
            {
               vals[num_vals++] = exprs->exprs.data[i].args[0].disp;
               // for now we assume 16 bits TODO
               has_modrm = true;
            }
            break;
         }
         break;
      case 2:
         break;
      default:  // no args, like "ret". nice and easy
         break;
      }

      u8 encoded_length = 1;
      if (has_modrm)
         ++encoded_length;
      encoded_length += num_vals * 2;  // TODO fix this to work with 8 bit values (*^ω^*)
      u64 ecbo = out.length;           // EnCodedBeginOffset
      grow_arr(out, encoded_length);
      out.data[ecbo++] = opcode;
      if (has_modrm)
         out.data[ecbo++] = modrm;
      for (u8 i = 0; i != num_vals; ++i)
      {
         out.data[ecbo++] = vals[i] & 0xFF;
         out.data[ecbo++] = (vals[i] >> 8) & 0xFF;
      }
   }

   return out;
}