#include <ctype.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LINE_MAX 128
#define LABEL_MAX 8
#define MAX_LABEL 256
#define ARG_MAX 32
#define MAX_ARG 2
#define REG_MAX 5
#define MAX_INSTRUC 1024
#define MAX_SPECIAL 512
#define MAX_BYTE 65536

#define BIT(n) (1 << n)

enum OpSize
{
   none = 0,
   byte = 1,
   word = 2,
   dword = 4,
   qword = 8,
};

enum OpSpec  // operand specifier
{
   reg = BIT(1),
   reg_or_mem = BIT(1) & BIT(2),
   mem = BIT(2),
   imm = BIT(3),
};

struct Register
{
   char name[REG_MAX];
   uint8_t opcode;
   enum OpSize size;
};

struct Label
{
   char name[LABEL_MAX];
   uint32_t offset;
};

struct ArgSpec
{
   enum OpSpec spec;
   enum OpSize size;
};

struct Argument
{
   struct ArgSpec spec;
   uint16_t disp;
   uint8_t base;
   uint8_t index;
};

struct Instruction
{
   char mnemonic[LABEL_MAX];
   uint8_t opcode;
};

enum SpecialType
{
   nospecial = 0,
   desig,
   org,
};

struct InstructionCall
{
   char mnemonic[LABEL_MAX];
   struct Argument args[MAX_ARG];
   uint16_t num_args;
   enum OpSize biggest_size;
   enum SpecialType special;
};

struct Register registers[] = {
    {"ax", 0, word}, {"cx", 1, word}, {"dx", 2, word}, {"bx", 3, word},
    {"sp", 4, word}, {"bp", 5, word}, {"si", 6, word}, {"di", 7, word},

    {"al", 0, byte}, {"cl", 1, byte}, {"dl", 2, byte}, {"bl", 3, byte},
    {"ah", 4, byte}, {"ch", 5, byte}, {"dh", 6, byte}, {"bh", 7, byte},
};
uint8_t num_registers = sizeof(registers) / sizeof(struct Register);

struct Instruction instructions[] = {
    {"add", 0x00}, {"mov", 0x88},  {"push", 0x50},  {"pop", 0x58},
    {"ret", 0xC3}, {"call", 0xE8}, {"callf", 0xA9},
};
uint16_t num_instructions = sizeof(instructions) / sizeof(struct Instruction);

struct
{
   const char* name;
   enum OpSize size;
} size_specs[] = {
    {"byte", byte},
    {"word", word},
    {"dword", dword},
    {"qword", qword},
};

enum UnitStatus
{
   MARKING_LABELS,
   ENCODING,
};

struct Unit
{
   struct InstructionCall instructions[MAX_INSTRUC];
   uint16_t num_Instructions;
   uint8_t bytes[MAX_BYTE];
   uint64_t num_Bytes;
   struct Label labels[MAX_LABEL];
   uint16_t num_Labels;

   enum UnitStatus status;
};

struct Unit* current_unit;

uint8_t set_size_spec(char* s, enum OpSize* out)  // check size specifier
{
   uint8_t len = 0;
   for (uint8_t i = 0; i != sizeof(size_specs) / sizeof(size_specs[0]); ++i)
   {
      len = strlen(size_specs[i].name);
      if (!memcmp(size_specs[i].name, s, len))
      {
         *out = size_specs[i].size;
         return len;
      }
   }
   return 0;
}

// struct Instruction* find_instruction(char* name) {}

uint8_t interp_reg(char* s, struct Argument* arg, uint8_t base_or_index)
{
   uint8_t len = 0;
   for (uint8_t i = 0; i != num_registers; ++i)
   {
      len = strlen(registers[i].name);
      if (!memcmp(registers[i].name, s, len))
      {
         if (base_or_index)
            arg->base = registers[i].opcode;
         else
            arg->index = registers[i].opcode;

         if (arg->spec.size < registers[i].size)
            arg->spec.size = registers[i].size;
         return len;
      }
   }
   return 0;
}

enum OpSize deduce_size(uint16_t size)
{
   if (size > UINT8_MAX)
      return word;
   else
      return byte;
}

uint8_t interp_imm(char* s, struct Argument* arg)
{
   char* eos = s;
   while (isdigit(eos[0]))
      ++eos;

   arg->disp = strtoull(s, &eos, 10);
   if (!arg->spec.size)
   {
      arg->spec.size = deduce_size(arg->disp);
   }
   return eos - s;
}

#define substrmatch(s1, s2) (!memcmp((s1), (s2), strlen(s2)))

void add_instruc(char name[LABEL_MAX], uint8_t num_args, char args[MAX_ARG][ARG_MAX])
{
   printf("%s with %hhu args: %s, %s\n", name, num_args, num_args > 0 ? args[0] : "",
          num_args == 2 ? args[1] : "");
   struct InstructionCall ins = {0};

   strcpy_s(ins.mnemonic, LABEL_MAX, name);
   ins.num_args = num_args;

   for (uint8_t i = 0; i != num_args; ++i)
   {
      char* aptr = args[i];
      aptr += set_size_spec(args[i], &ins.args[i].spec.size);
      while (isspace(aptr[0]))
         ++aptr;

      if (isdigit(aptr[0]))
      {
         ins.args[i].spec.spec = imm;
         aptr += interp_imm(aptr, &ins.args[i]);
      }
      else if (aptr[0] == '[')
      {
         ins.args[i].spec.spec = mem;
         uint8_t bis_reg = interp_reg(++aptr, &ins.args[i], 0);  // base
         // is it a register?
         if (bis_reg)
         {
            aptr += bis_reg;  // move the pointer
         }
         if (aptr[0] == '*')
         {
            ++aptr;
            bis_reg = interp_reg(aptr, &ins.args[i], 1);  // index
            // is it a register?
            if (bis_reg)
            {
               aptr += bis_reg;  // move the pointer
            }
         }
         if (aptr[0] == '+')
            ++aptr;
         // now check for a index and displacement
         // displacement
         if (isdigit(aptr[0]))
         {
            aptr += interp_imm(aptr, &ins.args[i]);
         }
         else
         {
            for (uint16_t i = 0; i != current_unit->num_Labels; ++i)
            {

               ins.args[i].spec.spec = imm;
               uint8_t offs = strlen(current_unit->labels[i].name);
               if (!memcmp(current_unit->labels[i].name, aptr, offs))
               {
                  ins.args[i].disp = current_unit->labels[i].offset;
                  ins.args[i].spec.size = word;
               }
               // interp label
               printf_s("absolute label named %s\n", aptr,
                        offs);  // this is not getting picked up forsome reason hence the lack of
                                // printing of this
            }
         }
      }
      // an issue with these labelings came to mind. the displacements will be wrong because the
      // labels may come after the instruction itself. I need to move the filling of these to after
      // estimating the bytelen for everything. I had thought this problem solved but inadvertently
      // progrmamed in the same mistake yet again. i had not noticed because i had zeroed out and
      // was for some reason testing with a label which itself was at offset 0
      // i have a potential solution but it's somewhat janky
      else if (aptr[0] == '|')  // relative label
      {
         ins.args[i].spec.spec = imm;  // later distinguish memory vs imm perhaps?
         for (uint16_t i = 0; i != current_unit->num_Labels; ++i)
         {
            uint8_t offs = strlen(current_unit->labels[i].name);
            if (!memcmp(current_unit->labels[i].name, aptr, offs))
            {
               ins.args[i].disp =
                   (uint16_t)(current_unit->num_Bytes - current_unit->labels[i].offset);
               // problem: i need to have a way to add the length to this. the possibility of some
               // sort of marker system seems appealing. or possibly deducing the type before
               // actually filing in the values. but that arises the problem of size deduction again
            }
         }
      }
      else
      {
         ins.args[i].spec.spec = reg;
         aptr += interp_reg(aptr, &ins.args[i], 0);  // store in base for register onlys
      }
   }

   for (uint8_t i = 0; i != num_args; ++i)
   {
      if (ins.args[i].spec.size > ins.biggest_size)
         ins.biggest_size = ins.args[i].spec.size;

      if (ins.args[i].spec.spec == imm)
      {
         printf("imm: %hu\n", ins.args[i].disp);
      }
      else if (ins.args[i].spec.spec == reg)
      {
         printf("reg: %hhu\n", ins.args[i].base);
      }
      else
      {
         printf("mem: %hhu, %hhu, %hu\n", ins.args[i].base, ins.args[i].index, ins.args[i].disp);
      }
   }

   uint8_t est_bytelen;
   if (num_args == 1)
   {
      est_bytelen = 1;
      switch (ins.args[0].spec.spec)
      {
      case imm:
      case mem:
         est_bytelen += ins.args[0].spec.size;
         break;
      default:
         break;
      }
   }
   else if (num_args == 2)
   {
      est_bytelen = 2;
      for (uint8_t i = 0; i != num_args; ++i)
      {
         switch (ins.args[i].spec.spec)
         {
         case mem:
         case imm:
            est_bytelen += ins.args[i].spec.size;
            break;
         default:
            break;
         }
      }
   }
   else
   {
      est_bytelen = 1;
   }

   printf("Specified size %hhu, total bytes: %hhu\n", ins.biggest_size, est_bytelen);

   current_unit->instructions[current_unit->num_Instructions++] = ins;
   current_unit->num_Bytes += est_bytelen;
}

void add_label(const char* mnem)
{
   strcpy_s(current_unit->labels[current_unit->num_Labels++].name, LABEL_MAX, mnem);
   printf("label %s at index %llu\n", mnem, current_unit->num_Bytes);
}

void interpret_line(char* line, bool labelling)
{
   if (line[0] == '.')
   {
      struct InstructionCall ins = {0};
      if (substrmatch(line + 1, "org"))
      {
         // num_bytes += interp_imm(line + whatever)
      }
      else if (line[1] == 'd')
      {
         ins.special = desig;
         switch (line[2])
         {
         case 'b':
            ins.biggest_size = byte;
            break;
         case 'w':
            ins.biggest_size = word;
            break;
         case 'd':  // not relevant here just here for reasons (no real reason)
            ins.biggest_size = dword;
            break;
         }

         ins.num_args = 1;
         interp_imm(line + 4, &ins.args[0]);
         current_unit->instructions[current_unit->num_Instructions++] = ins;
      }
      else if (substrmatch(line + 1, "times"))
      {
         // for loop, add the instruction n times as per interp_imm
      }
   }

   char mnem[LABEL_MAX];

   char args_str[MAX_ARG][ARG_MAX];

   uint8_t match = sscanf_s(line, "%s %[^,\n\t\r], %[^\n\t\r]", mnem, sizeof(mnem), args_str[0],
                            sizeof(args_str[0]), args_str[1], sizeof(args_str[1]));

   uint8_t mneml = strlen(mnem) - 1;
   if (mnem[mneml] == ':')
   {
      if (labelling)
      {
         mnem[mneml] = 0;
         add_label(mnem);
      }
   }
   else
   {
      add_instruc(mnem, match - 1, args_str);
   }
}
void write_imm_of_len(FILE** f, uint64_t val, enum OpSize size)
{
   for (uint8_t i = 0; i != size; ++i)
   {
      fputc((val >> (8 * i) & 0xFF), *f);
   }
}

uint8_t find_opc(const char* mnem)
{
   for (uint16_t i = 0; i != num_instructions; ++i)
   {
      if (!strcmp(instructions[i].mnemonic, mnem))
      {
         return instructions[i].opcode;
      }
   }
   printf("no opcode match for %s", mnem);
   exit(EXIT_FAILURE);
}

void assemble(const char* in_path, const char* out_path)
{
   struct Unit unit = {0};
   current_unit = &unit;
   FILE* in_file;
   fopen_s(&in_file, in_path, "rt");

   char line[LINE_MAX];
   while (fgets(line, LINE_MAX, in_file))
   {
      interpret_line(line, true);
   }
   printf("Current unit has %hu instructions, %hu labels, and a total byte length of %llu\n",
          unit.num_Instructions, unit.num_Labels, unit.num_Bytes);

   unit.num_Bytes = 0;
   unit.num_Instructions = 0;
   fseek(in_file, 0l, SEEK_SET);
   while (fgets(line, LINE_MAX, in_file))
   {
      interpret_line(line, false);
   }  // this may be the solution. going back to the beginning and retraversing. not very ideal but
      // a reasonable solution that can be refined later
   printf("Second pass complete. unit has %hu instructions, %hu labels, and a total byte length of "
          "%llu\n",
          unit.num_Instructions, unit.num_Labels, unit.num_Bytes);
   fclose(in_file);
   FILE* out_file;
   fopen_s(&out_file, out_path, "wb");

   for (uint16_t i = 0; i != unit.num_Instructions; ++i)
   {
      switch (unit.instructions[i].special)
      {
      case nospecial:
      {
         uint8_t opc = find_opc(unit.instructions[i].mnemonic);
         switch (unit.instructions[i].num_args)
         {
         case 0:
            fputc(opc, out_file);
            break;
         case 1:
         {
            switch (unit.instructions[i].args[0].spec.spec)
            {
            case reg:
               fputc(opc + unit.instructions[i].args[0].base, out_file);
               break;
            case imm:
               fputc(opc, out_file);
               write_imm_of_len(&out_file, unit.instructions[i].args[0].disp,
                                unit.instructions[i].biggest_size);
               break;
            default:
               break;
            }
         }
         break;
         case 2:
         {
            if (unit.instructions[i].args[0].spec.spec == reg)
            {
               if (unit.instructions[i].args[1].spec.spec == reg)
               {
                  //
               }
            }
         }
         break;
         }
      }
      break;
      case desig:
         write_imm_of_len(&out_file, unit.instructions[i].args[0].disp,
                          unit.instructions[i].biggest_size);
         break;
      case org:
         break;
      }
   }

   fclose(out_file);
   //
}

int main(void)
{
   for (int i = 0; i != 64; ++i)
      printf("\n");

   assemble("../tests/test.asm", "../tests/test.bin");

   return 0;
}
