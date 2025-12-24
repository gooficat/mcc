#pragma once
#include "arr.h"
#include "snips.h"

#include "as/expr_gen.h"

u8_arr as_encode(as_unencoded_unit ptr exprs);

bool mnem_match_expr(as_expr ptr expr, const mnem ptr mnem);

const mnem ptr find_expr_mnem(as_expr ptr expr);
