#include "mas.h"
#include "garr.h"

#include <ctype.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum toktype
{
   TK_REG = '%',
   TK_IMM = '$',
   TK_CMA = ',',
   TK_MEM = '[',

   TK_ADD = '+',
   TK_SUB = '-',
   TK_MUL = '*',
   TK_LBD = ':',

   TK_KEW = '?',

   TK_EOF = 'E',
};

struct token
{
   enum toktype type;
   size_t value;
};

struct token_arr array_struct(struct token);
struct iden_arr array_struct(char*);

struct token_unit
{
   struct token_arr tokens;
   struct iden_arr idens;
};

struct token_unit read_tokens(FILE* in)
{
   struct token_unit out;
   char buf[TK_MAX];
   int c = fgetc(in);
   init_arr(out.tokens);
   init_arr(out.idens);

   while (c != EOF)
   {
      if (isspace(c))
      {
         c = fgetc(in);
         continue;
      }
      else
      {
         struct token tok;
         unsigned char fetch_unchecked = 0;

         switch (c)
         {
         case '%':
         case '$':
            tok.type = c;
            fetch_unchecked &= 0b01;
            break;
         case ',':
         case ':':
            tok.type = c;
            tok.value = (size_t)-1;
            break;
         default:
            fetch_unchecked &= 0b11;
            break;
         }

         c = fgetc(in);
         if (fetch_unchecked & 0b01)
         {
            unsigned char bufl = 0;
            do
            {
               buf[bufl++] = c;
               c = fgetc(in);
            } while (isalnum(c));
            buf[bufl++] = 0;

            size_t i;
            for (i = 0; i != out.idens.length; ++i)
               if (!strcmp(buf, out.idens.data[i]))
                  break;

            tok.value = i;
            if (i == out.idens.length)
            {

               grow_arr(out.idens, 1);

               char* iden = malloc(bufl);
               memcpy(iden, buf, bufl);

               out.idens.data[out.idens.length - 1] = iden;
            }
         }

         if (fetch_unchecked & 0b10)
         {
            if (isalpha(buf[0]))
            {
               tok.type = TK_MEM;
            }
            else
            {
               tok.type = TK_KEW;
            }
         }

         grow_arr(out.tokens, 1);
         out.tokens.data[out.tokens.length - 1] = tok;
      }
   }
   grow_arr(out.tokens, 1);
   out.tokens.data[out.tokens.length - 1].type = TK_EOF;

   return out;
}

void as(const char* path)
{
   FILE* in;
   fopen_s(&in, path, "rt");
   struct token_unit tokens = read_tokens(in);

   for (size_t i = 0; i != tokens.tokens.length; ++i)
   {
      printf("tok: %c\n", tokens.tokens.data[i].type);
      if (tokens.tokens.data[i].value != (size_t)-1)
      {
         printf("value: %s\n", tokens.idens.data[tokens.tokens.data[i].value]);
      }
   }
   for (size_t i = 0; i != tokens.idens.length; ++i)
   {
      printf("identifier: %s\n", tokens.idens.data[i]);
   }
}
