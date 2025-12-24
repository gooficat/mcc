#include "as/as_spec.h"
#include "defs.h"

const reg_t regs[] = {
    {"ax", 0, sw}, {"cx", 1, sw}, {"dx", 2, sw}, {"bx", 3, sw},
    {"sp", 4, sw}, {"bp", 5, sw}, {"si", 6, sw}, {"di", 7, sw},
};
const u8 num_regs = sizeof(regs) / sizeof(regs[0]);

const mnem mnems[] = {
    {"add", {{rm, sb}, {reg, sb}}, 0x00},  //
    {"add", {{rm, sw}, {reg, sw}}, 0x01},  //
                                           //
    {"add", {{reg, sb}, {rm, sb}}, 0x02},  //
    {"add", {{reg, sw}, {rm, sw}}, 0x03},  //
                                           //
    {"pop", {{reg, sw}}, 0x58},            //
    {"mov", {{rm, sb}, {reg, sb}}, 0x88},  //
    {"ret", {}, 0xC3},                     //
    {"jmp", {{rel, sw}}, 0xE9},            //
    {"call", {{rel, sw}}, 0xEA},           //
};
const u8 num_mnems = sizeof(mnems) / sizeof(mnems[0]);
