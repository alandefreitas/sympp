//
// Created by Alan Freitas on 25/09/20.
//

#ifndef SYMPP_MATHEMATICS_H
#define SYMPP_MATHEMATICS_H

// C++
#include <iostream>

// Internal
#include <sympp/core/sym.h>
#include <sympp/node/statement/statement.h>
#include <sympp/node/terminal/variable.h>

namespace sympp {
    sym tan(const sym &);

    sym cot(const sym &);

    sym sec(const sym &);

    sym csc(const sym &);

   // This two functions was definided in specific file
   // sym sinh(const sym &);

   // sym cosh(const sym &);

    sym ln(const sym &);

    sym exp(const sym &);

    sym sqrt(const sym &);

    sym factorial(const sym &);

    sym gamma(const sym &);

    sym &rhs(std::vector<statement> &, const sym &);

    sym &lhs(std::vector<statement> &, const sym &);

    sym expand(const node_interface &);

} // namespace sympp

#endif // SYMPP_MATHEMATICS_H
