//
// Created by Alan Freitas on 25/09/20.
//

#ifndef SYMPP_OPERATORS_H
#define SYMPP_OPERATORS_H

// C++
#include <iostream>

// Internal
#include <sympp/core/sym.h>
#include <sympp/node/statement/statement.h>
#include <sympp/node/terminal/variable.h>

namespace sympp {

    std::ostream &operator<<(std::ostream &, const sym &);

    std::ostream &operator<<(std::ostream &, const node_interface &);

    sym operator+(const sym &);

    sym operator+(const sym &, const sym &);

    sym operator+(const node_interface &, const sym &);

    sym operator+(const sym &, const node_interface &);

    sym operator+(const std::vector<sym> &, const std::vector<sym> &);

    sym operator+(const std::vector<sym> &, const sym &);

    sym operator+(const sym &, const std::vector<sym> &);

    sym operator+(const int &, const sym &);

    sym operator+(const sym &, const int &);

    sym operator+(const double &, const sym &);

    sym operator+(const sym &, const double &);

    sym &operator++(sym &);

    const sym operator++(sym &, int);

    sym &operator+=(sym &, const sym &);

    sym &operator+=(sym &, const int &);

    sym &operator+=(sym &, const double &);

    sym &operator+=(std::vector<sym> &s1, sym &s2);

    sym &operator+=(sym &s1, const std::vector<sym> &s2);

    sym &operator+=(std::vector<sym> &s1, std::vector<sym> &s2);

    sym operator-(const sym &);

    sym operator-(const sym &, const sym &);

    sym operator-(const node_interface &, const sym &);

    sym operator-(const sym &, const node_interface &);

    sym operator-(const int &, const sym &);

    sym operator-(const sym &, const int &);

    sym operator-(const double &, const sym &);

    sym operator-(const sym &, const double &);

    sym &operator--(sym &);

    const sym operator--(sym &, int);

    sym &operator-=(sym &, const sym &);

    sym &operator-=(sym &, const int &);

    sym &operator-=(sym &, const double &);

    sym operator*(const sym &, const sym &);

    sym operator*(const node_interface &, const sym &);

    sym operator*(const sym &, const node_interface &);

    sym operator*(const int &, const sym &);

    sym operator*(const sym &, const int &);

    sym operator*(const double &, const sym &);

    sym operator*(const sym &, const double &);

    sym &operator*=(sym &, const sym &);

    sym &operator*=(sym &, const int &);

    sym &operator*=(sym &, const double &);

    sym operator/(const sym &, const sym &);

    sym operator/(const node_interface &, const sym &);

    sym operator/(const sym &, const node_interface &);

    sym operator/(const int &, const sym &);

    sym operator/(const sym &, const int &);

    sym operator/(const double &, const sym &);

    sym operator/(const sym &, const double &);

    sym &operator/=(sym &, const sym &);

    sym &operator/=(sym &, const int &);

    sym &operator/=(sym &, const double &);

    sym operator^(const sym &, const sym &);

    sym operator^(const node_interface &, const sym &);

    sym operator^(const sym &, const node_interface &);

    sym operator^(const sym &, int);

    sym operator^(int, const sym &);

    sym operator^(const sym &, double);

    sym operator^(double, const sym &);

    bool operator==(const sym &, const sym &);

    bool operator==(const node_interface &, const sym &);

    bool operator==(const sym &, const node_interface &);

    bool operator==(const sym &, int);

    bool operator==(int, const sym &);

    bool operator==(const sym &, double);

    bool operator==(double, const sym &);

    bool operator!=(const sym &, const sym &);

    bool operator!=(const node_interface &, const sym &);

    bool operator!=(const sym &, const node_interface &);

    bool operator!=(const sym &, int);

    bool operator!=(int, const sym &);

    bool operator!=(const sym &, double);

    bool operator!=(double, const sym &);

    bool operator<=(const sym &, const sym &);

    bool operator<=(const node_interface &, const sym &);

    bool operator<=(const sym &, const node_interface &);

    bool operator<=(const sym &, int);

    bool operator<=(int, const sym &);

    bool operator<=(const sym &, double);

    bool operator<=(double, const sym &);

    bool operator>=(const sym &, const sym &);

    bool operator>=(const node_interface &, const sym &);

    bool operator>=(const sym &, const node_interface &);

    bool operator>=(const sym &, int);

    bool operator>=(int, const sym &);

    bool operator>=(const sym &, double);

    bool operator>=(double, const sym &);

    bool operator>(const sym &, const sym &);

    bool operator>(const node_interface &, const sym &);

    bool operator>(const sym &, const node_interface &);

    bool operator>(const sym &, int);

    bool operator>(int, const sym &);

    bool operator>(const sym &, double);

    bool operator>(double, const sym &);

    bool operator<(const sym &, const sym &);

    bool operator<(const node_interface &, const sym &);

    bool operator<(const sym &, const node_interface &);

    bool operator<(const sym &, int);

    bool operator<(int, const sym &);

    bool operator<(const sym &, double);

    bool operator<(double, const sym &);

} // namespace sympp

#endif // SYMPP_OPERATORS_H
