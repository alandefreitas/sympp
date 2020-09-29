//
// Created by Alan Freitas on 25/09/20.
//

#ifndef SYMPP_SYMBOLIC_H
#define SYMPP_SYMBOLIC_H

// C++
#include <iostream>

// Internal
#include <sympp/core/sym.h>
#include <sympp/node/statement/statement.h>
#include <sympp/node/terminal/variable.h>

namespace sympp {

    /// Equal
    sym eq(const sym &, const sym &);
    sym eq(int, const sym &);
    sym eq(const sym &, int);
    sym eq(double, const sym &);
    sym eq(const sym &, double);

    /// Not equal
    sym neq(const sym &, const sym &);
    sym neq(int, const sym &);
    sym neq(const sym &, int);
    sym neq(double, const sym &);
    sym neq(const sym &, double);

    /// Less than or equal to
    sym leq(const sym &, const sym &);
    sym leq(int, const sym &);
    sym leq(const sym &, int);
    sym leq(double, const sym &);
    sym leq(const sym &, double);

    /// Less than
    sym lt(const sym &, const sym &);
    sym lt(int, const sym &);
    sym lt(const sym &, int);
    sym lt(double, const sym &);
    sym lt(const sym &, double);

    /// Greater than or equal to
    sym geq(const sym &, const sym &);
    sym geq(int, const sym &);
    sym geq(const sym &, int);
    sym geq(double, const sym &);
    sym geq(const sym &, double);

    /// Greater than
    sym gt(const sym &, const sym &);
    sym gt(int, const sym &);
    sym gt(const sym &, int);
    sym gt(double, const sym &);
    sym gt(const sym &, double);

    sym &expand(sym &);

    sym zero();

    sym one();

} // namespace sympp

#endif // SYMPP_SYMBOLIC_H
