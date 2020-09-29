//
// Created by Alan Freitas on 25/09/20.
//

#include "symbolic.h"
#include <sympp/core/sym.h>
#include <sympp/node/statement/statement.h>

namespace sympp {
    /// Equal
    sym eq(const sym &s1, const sym &s2) {
        return sym(statement(s1, s2, statement::statement_type::equality));
    }

    sym eq(int s1, const sym &s2) { return eq(sym(s1), s2); }

    sym eq(const sym &s1, int s2) { return eq(s1, sym(s2)); }

    sym eq(double s1, const sym &s2) { return eq(sym(s1), s2); }

    sym eq(const sym &s1, double s2) { return eq(s1, sym(s2)); }

    /// Not equal
    sym neq(const sym &s1, const sym &s2) {
        return sym(statement(s1, s2, statement::statement_type::inequality));
    }

    sym neq(int s1, const sym &s2) { return neq(sym(s1), s2); }

    sym neq(const sym &s1, int s2) { return neq(s1, sym(s2)); }

    sym neq(double s1, const sym &s2) { return neq(sym(s1), s2); }

    sym neq(const sym &s1, double s2) { return neq(s1, sym(s2)); }

    /// Less than or equal to
    sym leq(const sym &s1, const sym &s2) {
        return sym(statement(s1, s2, statement::statement_type::less_equal));
    }

    sym leq(int s1, const sym &s2) { return leq(sym(s1), s2); }

    sym leq(const sym &s1, int s2) { return leq(s1, sym(s2)); }

    sym leq(double s1, const sym &s2) { return leq(sym(s1), s2); }

    sym leq(const sym &s1, double s2) { return leq(s1, sym(s2)); }

    /// Less than
    sym lt(const sym &s1, const sym &s2) {
        return sym(statement(s1, s2, statement::statement_type::less_than));
    }

    sym lt(int s1, const sym &s2) { return lt(sym(s1), s2); }

    sym lt(const sym &s1, int s2) { return lt(s1, sym(s2)); }

    sym lt(double s1, const sym &s2) { return lt(sym(s1), s2); }

    sym lt(const sym &s1, double s2) { return lt(s1, sym(s2)); }

    /// Greater than or equal to
    sym geq(const sym &s1, const sym &s2) {
        return sym(statement(s1, s2, statement::statement_type::greater_equal));
    }

    sym geq(int s1, const sym &s2) { return geq(sym(s1), s2); }

    sym geq(const sym &s1, int s2) { return geq(s1, sym(s2)); }

    sym geq(double s1, const sym &s2) { return geq(sym(s1), s2); }

    sym geq(const sym &s1, double s2) { return geq(s1, sym(s2)); }

    /// Greater than
    sym gt(const sym &s1, const sym &s2) {
        return sym(statement(s1, s2, statement::statement_type::greater_equal));
    }

    sym gt(int s1, const sym &s2) { return gt(sym(s1), s2); }

    sym gt(const sym &s1, int s2) { return gt(s1, sym(s2)); }

    sym gt(double s1, const sym &s2) { return gt(sym(s1), s2); }

    sym gt(const sym &s1, double s2) { return gt(s1, sym(s2)); }

    sym &expand(sym &s) {
        s.expand();
        return s;
    }

    sym zero() { return sym(0); }

    sym one() { return sym(1); }

} // namespace sympp