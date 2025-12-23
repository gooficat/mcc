#include "as/as_tk.h"

int main()
{
   init_fancc;
   fancc(YELLOW, "Hello, World!\n");

   fstream_t fs;
   fsopen(fs, "../tests/test1.s", FMODE_RT);

   as_tok_arr toks = as_fetch_tokens(addr fs);

   for (u64 i = 0; i != toks.length; ++i)
   {
      fancc(PURPLE, "\n%c", toks.data[i].type);
      if (toks.data[i].value != NULL)
      {
         fancc(BLUE, ":\t%s", toks.data[i].value);
      }
   }

   return 0;
}
