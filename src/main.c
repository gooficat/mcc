#include <ctype.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#define LINE_MAX 128
#define MAX_LABELS 64
#define LABEL_MAX 32

#define ARG_MAX 32
#define MAX_ARGS 2

static const struct reg
{
   const char* name;
   uint8_t opcode;
   uint8_t size;
} regs[] = {
    {"ax", 0, 2},
    {"cx", 1, 2},
    {"dx", 2, 2},
    {"bx", 3, 2},

};
uint8_t num_regs = sizeof(regs) / sizeof(regs[0]);

int8_t find_register(const char* name)
{
   uint8_t i = 0;
   while (strcmp(regs[i++].name, name))
      if (i == num_regs)
         return num_regs;

   return regs[i].opcode;
}

int main(void)
{
   FILE *in, *out;
   fopen_s(&in, "../tests/test.s", "rt");
   fopen_s(&out, "../tests/test.bin", "wb");

   struct label
   {
      char name[LABEL_MAX];
      uint16_t offset;
   } labels[MAX_LABELS];
   uint8_t num_labels = 0;

   char line[LINE_MAX];

   char mnem[LABEL_MAX];
   char args[MAX_ARGS][ARG_MAX];
   uint8_t num_args;

   uint16_t bytes_written = 0;

   while (fgets(line, LINE_MAX, in))
   {
      printf("%s", line);
      num_args = sscanf_s(line, "%s %[^,\n\r\t ], %[^\n\r\t ]", mnem, LABEL_MAX, args[0], ARG_MAX,
                          args[1], ARG_MAX) -
                 1;

      uint8_t mend = strlen(mnem) - 1;
      if (mnem[mend] == ':')
      {
         mnem[mend] = 0;
         printf("label %s at %hu\n", mnem, bytes_written);
         strcpy_s(labels[num_labels++].name, LABEL_MAX, mnem);
         continue;  // TODO
      }
      uint8_t ins_byte_len;

      switch (num_args)
      {
      case 0:
         ins_byte_len = 1;
         break;
      case 1:
         if (find_register(args[0]) != num_regs)
            ins_byte_len = 2;
         else
            ins_byte_len = 3;  // be it memory or immediate, label or not
         break;
      case 2:
      {
         uint8_t oa = (find_register(args[0]) != num_regs) ? 1 : 0;

         if (isdigit(args[oa][0]))
            ins_byte_len = 5;
         else if (find_register(args[oa]) != num_regs)
            ins_byte_len = 2;
         else if (args[oa][0] == '[')
         {
            if (strchr(args[oa], '+'))
               ins_byte_len = 5;
            else
               ins_byte_len = 3;
         }
         else
         {
            if (strchr(args[oa], '+'))
               ins_byte_len = 5;
            else
               ins_byte_len = 3;
         }
      }
      break;
      }
      bytes_written += ins_byte_len;
   }
   return 0;
}
