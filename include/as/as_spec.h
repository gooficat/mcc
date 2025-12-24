#pragma once

#include "snips.h"

struc_t
{
   const str name;
   u8 code;
}
reg;

struc_t
{
   const str name;
   u8 code;
}
mnem;

extern const reg regs[];
extern const mnem mnems[];
extern const u8 num_regs;
extern const u8 num_mnems;
