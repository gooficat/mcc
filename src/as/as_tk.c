#include "as/as_tk.h"
#include "arr.h"
#include "file.h"

as_tok_arr as_fetch_tokens(fstream_t ptr fs)
{
   as_tok_arr toks;
   as_tok tok;
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
      bool has_value;
      switch (fs->c)
      {
      case '%':
      case '$':
         tok.type = fs->c;
         has_value = true;
         fsgetc(deref fs);
         break;
      case ',':
      case '+':
      case '-':
      case '*':
      case ':':
         tok.type = fs->c;
         has_value = false;
         break;
      default:
         tok.type = AS_KEW;  // label, memory address, or instruction
         has_value = true;
         break;
      }

      if (has_value)
      {
         fetch_token(fs, buf);
         tok.value = copy_str(buf);
      }
      else
      {
         fsgetc(deref fs);
         tok.value = NULL;
      }

      grow_arr(toks, 1);
      toks.data[toks.length - 1] = tok;
   }
   grow_arr(toks, 1);
   tok.type = AS_EOF;
   tok.value = NULL;

   toks.data[toks.length - 1] = tok;
   return toks;
}
