#pragma once

#include "snips.h"

// since im doing 8086, i dont need dword/qword but its just here for emphasis and possibly the
// future
enu_m{
    sb = 1,
    sw = 2,
    sd = 4,
    sq = 8,
} byte_size;

struc_t
{
   const str name;
   u8 code;
   u8 size;
}
reg_t;

enu_m{
    Na = 0,  // no arg
    mem,    reg, rm, imm, rel,
} arg_spec_type;

struc_t
{
   arg_spec_type type;
   byte_size size;
}
arg_spec;

struc_t
{
   const str name;
   const arg_spec spec[2];
   u8 code;
}
mnem;

extern const reg_t regs[];
extern const mnem mnems[];
extern const u8 num_regs;
extern const u8 num_mnems;
