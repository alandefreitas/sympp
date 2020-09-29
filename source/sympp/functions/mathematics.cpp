//
// Created by Alan Freitas on 25/09/20.
//

#include "mathematics.h"
#include <sympp/core/sym.h>
#include <sympp/functions/operators.h>
#include <sympp/node/function/cos.h>
#include <sympp/node/function/log.h>
#include <sympp/node/function/sin.h>
#include <sympp/node/terminal/constant.h>

namespace sympp {

    sym tan(const sym &s) { return sym(sin(s)) / sym(cos(s)); }

    sym cot(const sym &s) { return sym(cos(s)) / sym(sin(s)); }

    sym sec(const sym &s) { return 1 / sym(cos(s)); }

    sym csc(const sym &s) { return 1 / sym(sin(s)); }

    sym ln(const sym &s) { return sym(log(constant::e(), s)); }

    sym exp(const sym &s) { return constant::e() ^ s; }

    sym sqrt(const sym &s) { return s ^ (0.5); }

} // namespace sympp