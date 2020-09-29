//
// Created by Alan Freitas on 25/09/20.
//

#include "operators.h"
#include <sympp/core/sym.h>
#include <sympp/functions/operators.h>
#include <sympp/node/function/cos.h>
#include <sympp/node/function/log.h>
#include <sympp/node/function/pow.h>
#include <sympp/node/function/sin.h>
#include <sympp/node/operation/summation.h>
#include <sympp/node/terminal/constant.h>

namespace sympp {
    std::ostream &operator<<(std::ostream &o, const sym &s) {
        s.stream(o, true);
        return o;
    }

    std::ostream &operator<<(std::ostream &o, const node_interface &s) {
        s.stream(o, true);
        return o;
    }

    sym operator+(const sym &s) { return s; }

    sym operator+(const sym &s1, const sym &s2) {
        return sym(summation(s1, s2));
    }

    sym operator+(const node_interface &s1, const sym &s2) {
        return sym(summation(sym(s1), s2));
    }

    sym operator+(const sym &s1, const node_interface &s2) {
        return sym(summation(s1, sym(s2)));
    }

    sym operator+(const int &s1, const sym &s2) { return sym(s1) + s2; }

    sym operator+(const sym &s1, const int &s2) { return s1 + sym(s2); }

    sym operator+(const double &s1, const sym &s2) { return sym(s1) + s2; }

    sym operator+(const sym &s1, const double &s2) { return s1 + sym(s2); }

    sym &operator++(sym &s) {
        s = s + 1;
        return s;
    }

    const sym operator++(sym &s, int) { // NOLINT
        sym t = s;
        ++s;
        return t;
    }

    sym &operator+=(sym &s1, const sym &s2) {
        s1 = s1 + s2;
        return s1;
    }

    sym &operator+=(sym &s1, const int &s2) {
        s1 = s1 + s2;
        return s1;
    }

    sym &operator+=(sym &s1, const double &s2) {
        s1 = s1 + s2;
        return s1;
    }

    sym operator-(const sym &s) { return sym(product(sym(-1), s)); }

    sym operator-(const sym &s1, const sym &s2) {
        return sym(summation(s1, -s2));
    }

    sym operator-(const node_interface &s1, const sym &s2) {
        return sym(summation(sym(s1), -s2));
    }

    sym operator-(const sym &s1, const node_interface &s2) {
        return sym(summation(s1, -sym(s2)));
    }

    sym operator-(const int &s1, const sym &s2) { return sym(s1) - s2; }

    sym operator-(const sym &s1, const int &s2) { return s1 - sym(s2); }

    sym operator-(const double &s1, const sym &s2) { return sym(s1) - s2; }

    sym operator-(const sym &s1, const double &s2) { return s1 - sym(s2); }

    sym &operator--(sym &s) {
        s = s - 1;
        return s;
    }

    const sym operator--(sym &s, int) { // NOLINT
        sym t = s;
        --s;
        return t;
    }

    sym &operator-=(sym &s1, const sym &s2) {
        s1 = s1 - s2;
        return s1;
    }

    sym &operator-=(sym &s1, const int &s2) {
        s1 = s1 - s2;
        return s1;
    }

    sym &operator-=(sym &s1, const double &s2) {
        s1 = s1 - s2;
        return s1;
    }

    sym operator*(const sym &s1, const sym &s2) { return sym(product(s1, s2)); }

    sym operator*(const node_interface &s1, const sym &s2) {
        return sym(product(sym(s1), s2));
    }

    sym operator*(const sym &s1, const node_interface &s2) {
        return sym(product(s1, sym(s2)));
    }

    sym operator*(const int &s1, const sym &s2) { return sym(s1) * s2; }

    sym operator*(const sym &s1, const int &s2) { return s1 * sym(s2); }

    sym operator*(const double &s1, const sym &s2) { return sym(s1) * s2; }

    sym operator*(const sym &s1, const double &s2) { return s1 * sym(s2); }

    sym &operator*=(sym &s1, const sym &s2) {
        s1 = s1 * s2;
        return s1;
    }

    sym &operator*=(sym &s1, const int &s2) {
        s1 = s1 * s2;
        return s1;
    }

    sym &operator*=(sym &s1, const double &s2) {
        s1 = s1 * s2;
        return s1;
    }

    sym operator/(const sym &s1, const sym &s2) {
        return sym(product(s1, sym(pow(s2, sym(-1)))));
    }

    sym operator/(const node_interface &s1, const sym &s2) {
        return sym(product(sym(s1), sym(pow(s2, sym(-1)))));
    }

    sym operator/(const sym &s1, const node_interface &s2) {
        return sym(product(sym(s1), sym(pow(sym(s2), sym(-1)))));
    }

    sym operator/(const int &s1, const sym &s2) { return sym(s1) / s2; }

    sym operator/(const sym &s1, const int &s2) { return s1 / sym(s2); }

    sym operator/(const double &s1, const sym &s2) { return sym(s1) / s2; }

    sym operator/(const sym &s1, const double &s2) { return s1 / sym(s2); }

    sym &operator/=(sym &s1, const sym &s2) {
        s1 = s1 / s2;
        return s1;
    }

    sym &operator/=(sym &s1, const int &s2) {
        s1 = s1 / s2;
        return s1;
    }

    sym &operator/=(sym &s1, const double &s2) {
        s1 = s1 / s2;
        return s1;
    }

    bool operator==(const sym &s1, const sym &s2) {
        return s1.compare(s2) == 0;
    }

    bool operator==(const node_interface &s1, const sym &s2) {
        return s1.compare(*s2.root_node()) == 0;
    }

    bool operator==(const sym &s1, const node_interface &s2) {
        return s1.compare(s2) == 0;
    }

    bool operator==(const sym &s1, int n) { return s1.compare(sym(n)) == 0; }

    bool operator==(int n, const sym &s2) { return (sym(n)).compare(s2) == 0; }

    bool operator==(const sym &s1, double n) { return s1.compare(sym(n)) == 0; }

    bool operator==(double n, const sym &s2) {
        return (sym(n)).compare(s2) == 0;
    }

    bool operator!=(const sym &s1, const sym &s2) {
        return s1.compare(s2) != 0;
    }

    bool operator!=(const node_interface &s1, const sym &s2) {
        return s1.compare(*s2.root_node()) != 0;
    }

    bool operator!=(const sym &s1, const node_interface &s2) {
        return s1.compare(s2) != 0;
    }

    bool operator!=(const sym &s1, int n) { return s1.compare(sym(n)) != 0; }

    bool operator!=(int n, const sym &s2) { return (sym(n)).compare(s2) != 0; }

    bool operator!=(const sym &s1, double n) { return s1.compare(sym(n)) != 0; }

    bool operator!=(double n, const sym &s2) {
        return (sym(n)).compare(s2) != 0;
    }

    bool operator<=(const sym &s1, const sym &s2) {
        return s1.compare(s2) <= 0;
    }

    bool operator<=(const node_interface &s1, const sym &s2) {
        return s1.compare(*s2.root_node()) <= 0;
    }

    bool operator<=(const sym &s1, const node_interface &s2) {
        return s1.compare(s2) <= 0;
    }

    bool operator<=(const sym &s1, int n) { return s1.compare(sym(n)) <= 0; }

    bool operator<=(int n, const sym &s2) { return (sym(n)).compare(s2) <= 0; }

    bool operator<=(const sym &s1, double n) { return s1.compare(sym(n)) <= 0; }

    bool operator<=(double n, const sym &s2) {
        return (sym(n)).compare(s2) <= 0;
    }

    bool operator>=(const sym &s1, const sym &s2) {
        return s1.compare(s2) >= 0;
    }

    bool operator>=(const node_interface &s1, const sym &s2) {
        return s1.compare(*s2.root_node()) >= 0;
    }

    bool operator>=(const sym &s1, const node_interface &s2) {
        return s1.compare(s2) >= 0;
    }

    bool operator>=(const sym &s1, int n) { return s1.compare(sym(n)) >= 0; }

    bool operator>=(int n, const sym &s2) { return (sym(n)).compare(s2) >= 0; }

    bool operator>=(const sym &s1, double n) { return s1.compare(sym(n)) >= 0; }

    bool operator>=(double n, const sym &s2) {
        return (sym(n)).compare(s2) >= 0;
    }

    bool operator>(const sym &s1, const sym &s2) { return s1.compare(s2) > 0; }

    bool operator>(const node_interface &s1, const sym &s2) {
        return s1.compare(*s2.root_node()) > 0;
    }

    bool operator>(const sym &s1, const node_interface &s2) {
        return s1.compare(s2) > 0;
    }

    bool operator>(const sym &s1, int n) { return s1.compare(sym(n)) > 0; }

    bool operator>(int n, const sym &s2) { return (sym(n)).compare(s2) > 0; }

    bool operator>(const sym &s1, double n) { return s1.compare(sym(n)) > 0; }

    bool operator>(double n, const sym &s2) { return (sym(n)).compare(s2) > 0; }

    bool operator<(const sym &s1, const sym &s2) { return s1.compare(s2) < 0; }

    bool operator<(const node_interface &s1, const sym &s2) {
        return s1.compare(*s2.root_node()) < 0;
    }

    bool operator<(const sym &s1, const node_interface &s2) {
        return s1.compare(s2) < 0;
    }

    bool operator<(const sym &s1, int n) { return s1.compare(sym(n)) < 0; }

    bool operator<(int n, const sym &s2) { return (sym(n)).compare(s2) < 0; }

    bool operator<(const sym &s1, double n) { return s1.compare(sym(n)) < 0; }

    bool operator<(double n, const sym &s2) { return (sym(n)).compare(s2) < 0; }

} // namespace sympp