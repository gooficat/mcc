#include "as/as_tk.h"
#include "arr.h"
#include "file.h"

as_tok_arr as_fetch_tokens(fstream_t ptr fs)
{
   as_tok_arr toks;
   char buf[TK_MAX];
   u8 tk_len;
   init_arr(toks);
   while (not fseof(deref fs))
   {
      if (isspace(fs->c))
      {
         fsgetc(deref fs);
         continue;
      }
      as_tok tok;

      switch (fs->c)
      {
      case '%':
         tok.type = AS_REG;
         fsgetc(deref fs);
         fetch_token(fs, buf);
         tok.value = copy_str(buf);
         break;
      default:
         fsgetc(deref fs);
         break;
      }

      grow_arr(toks, 1);
      toks.data[toks.length - 1] = tok;
   }

   return toks;
}
