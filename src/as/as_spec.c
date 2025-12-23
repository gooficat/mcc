#include "as/as_spec.h"

const reg regs[] = {
    {"ax", 0}, {"cx", 1}, {"dx", 2}, {"bx", 3}, {"sp", 4}, {"bp", 5}, {"si", 6}, {"di", 7},
};
const u8 num_regs = sizeof(regs) / sizeof(regs[0]);

const mnem mnems[] = {
    {"add", 0x00},
    {"mov", 0x88},
};
const u8 num_mnems = sizeof(mnems) / sizeof(mnems[0]);
