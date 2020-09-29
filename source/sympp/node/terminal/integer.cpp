//
// Created by Alan Freitas on 20/09/20.
//

#include "integer.h"
#include "number_interface.h"

#include <sympp/core/sym_error.h>
#include <sympp/node/function/pow.h>
#include <sympp/node/operation/product.h>
#include <sympp/node/operation/summation.h>
#include <sympp/node/terminal/boolean.h>
#include <sympp/node/terminal/rational.h>
#include <sympp/node/terminal/real.h>

namespace sympp {
    integer::integer() : number_(0) {}
    integer::integer(const integer &v) : number_(v.number_) {}
    integer::integer(integer &&v) noexcept : number_(v.number_) {}
    integer::integer(bool v) : number_(static_cast<int>(v)) {}
    integer::integer(int v) : number_(v) {}
    integer::integer(double v) : number_(static_cast<int>(v)) {}
    integer::integer(const number_interface &v)
        : integer(static_cast<int>(v)) {}
    integer::integer(const node_interface &v)
        : integer(dynamic_cast<const number_interface &>(v)) {}
    integer::integer(const sym &v) : integer(*v.root_node()) {}
    integer::~integer() = default;

    double integer::evaluate(const std::vector<uint8_t> &,
                             const std::vector<int> &,
                             const std::vector<double> &) const {
        return static_cast<double>(number_);
    }

    sym integer::evaluate_sym(const std::vector<uint8_t> &,
                              const std::vector<int> &,
                              const std::vector<double> &) const {
        return sym(*this);
    }

    node_lambda integer::lambdify() const {
        // the lambda version of this returns this integer as a constant
        auto d = static_cast<double>(this->number_);
        return [d](const std::vector<uint8_t> &, const std::vector<int> &,
                   const std::vector<double> &) -> double { return d; };
    }

    void integer::c_code(int &, int &, int &, bool &leaf,
                         std::stack<std::string> &code_aux,
                         std::string &code) const {
        auto d = static_cast<double>(this->number_);
        if (leaf) {
            code_aux.push(std::to_string(d));
        } else {
            code += std::to_string(d);
        }
    }

    void integer::stream(std::ostream &o, bool) const { o << number_; }

    const std::type_info &integer::type() const {
        return typeid(decltype(*this));
    }

    node_interface *integer::clone() const {
        return dynamic_cast<node_interface *>(new integer(*this));
    }

    const std::type_info &integer::numeric_type() const { return typeid(int); }

    bool integer::is_zero() const { return number_ == 0; }

    bool integer::is_one() const { return number_ == 1; }

    bool integer::is_negative() const { return number_ < 0; }

    int integer::compare_number(const number_interface &rhs) const {
        if (rhs.type() == typeid(real)) {
            auto lhs_double = static_cast<double>(number_);
            auto rhs_double = static_cast<double>(rhs);
            if (lhs_double < rhs_double) {
                return -1;
            } else if (lhs_double > rhs_double) {
                return +1;
            } else {
                return 0;
            }
        } else if (rhs.type() == typeid(integer)) {
            auto rhs_int = static_cast<int>(rhs);
            if (number_ < rhs_int) {
                return -1;
            } else if (number_ > rhs_int) {
                return +1;
            } else {
                return 0;
            }
        } else if (rhs.type() == typeid(boolean)) {
            auto rhs_bool = static_cast<bool>(rhs);
            if (!rhs_bool) {
                if (number_ < 0) {
                    return -1;
                } else if (number_ > 0) {
                    return +1;
                } else {
                    return 0;
                }
            } else {
                if (number_ < 1) {
                    return -1;
                } else if (number_ > 1) {
                    return +1;
                } else {
                    return 0;
                }
            }
        } else if (rhs.type() == typeid(rational)) {
            const auto &r = dynamic_cast<const rational &>(rhs);
            int lhs_int = number_ * r.denominator();
            if (lhs_int > r.numerator()) {
                return -1;
            } else if (lhs_int < r.numerator()) {
                return +1;
            } else {
                return 0;
            }
        }
        return 0;
    }

    integer::operator bool() const { return number_ != 0; }

    integer::operator int() const { return number_; }

    integer::operator double() const { return static_cast<double>(number_); }

    sym integer::add(const number_interface &rhs) const {
        if (rhs.type() == typeid(boolean) || rhs.type() == typeid(integer)) {
            // might promote the boolean
            auto rhs_int = static_cast<int>(rhs);
            return sym(integer(number_ + rhs_int));
        } else {
            // Delegate commutative function to higher level
            return rhs.add(*this);
        }
    }

    sym integer::sub(const number_interface &rhs) const {
        if (rhs.type() == typeid(boolean)) {
            auto rhs_bool = static_cast<bool>(rhs);
            if (rhs_bool) {
                return sym(integer(number_ - 1));
            } else {
                return sym(integer(number_));
            }
        } else if (rhs.type() == typeid(integer)) {
            auto rhs_int = static_cast<int>(rhs);
            return sym(integer(number_ - rhs_int));
        } else if (rhs.type() == typeid(real)) {
            auto lhs_double = static_cast<double>(number_);
            auto rhs_double = static_cast<double>(rhs);
            return sym(real(lhs_double - rhs_double));
        } else if (rhs.type() == typeid(rational)) {
            const auto &r = dynamic_cast<const rational &>(rhs);
            int lhs_numerator = number_ * r.denominator();
            int result_numerator = lhs_numerator - r.numerator();
            if (result_numerator % r.denominator() == 0) {
                return sym(integer(result_numerator / r.denominator()));
            } else {
                return sym(rational(result_numerator, r.denominator()));
            }
        } else {
            return sym(summation(sym(*this), sym(product(sym(-1), sym(rhs)))));
        }
    }

    sym integer::mul(const number_interface &rhs) const {
        if (rhs.type() == typeid(boolean) || rhs.type() == typeid(integer)) {
            auto rhs_int = static_cast<int>(rhs);
            return sym(integer(number_ * rhs_int));
        } else {
            // Delegate commutative op
            return rhs.mul(*this);
        }
    }

    sym integer::div(const number_interface &rhs) const {
        if (rhs.type() == typeid(boolean)) {
            auto rhs_bool = static_cast<bool>(rhs);
            if (!rhs_bool) {
                return sym(integer(std::numeric_limits<int>::infinity()));
            } else {
                return sym(integer(number_));
            }
        } else if (rhs.type() == typeid(integer)) {
            auto rhs_int = static_cast<int>(rhs);
            if (rhs_int != 0 && number_ % rhs_int == 0) {
                return sym(integer(number_ / rhs_int));
            } else {
                return sym(rational(number_, rhs_int));
            }
        } else if (rhs.type() == typeid(real)) {
            auto lhs_double = static_cast<double>(number_);
            auto rhs_double = static_cast<double>(rhs);
            return sym(real(lhs_double / rhs_double));
        } else if (rhs.type() == typeid(rational)) {
            const auto &r = dynamic_cast<const rational &>(rhs);
            int result_numerator = number_ * r.denominator();
            if (result_numerator % r.numerator() == 0) {
                return sym(integer(result_numerator / r.numerator()));
            } else {
                return sym(rational(result_numerator, r.numerator()));
            }
        } else {
            return sym(product(sym(*this), sym(sympp::pow(sym(rhs), sym(-1)))));
        }
    }

    sym integer::mod(const number_interface &rhs) const {
        if (rhs.type() == typeid(boolean)) {
            auto rhs_bool = static_cast<bool>(rhs);
            if (!rhs_bool) {
                return sym(rational(static_cast<int>(number_), 0));
            } else {
                return sym(integer(0));
            }
        } else if (rhs.type() == typeid(integer)) {
            auto rhs_int = static_cast<int>(rhs);
            return sym(integer(number_ % rhs_int));
        } else if (rhs.type() == typeid(real)) {
            auto lhs_double = static_cast<double>(number_);
            auto rhs_double = static_cast<double>(rhs);
            return sym(real(std::fmod(lhs_double, rhs_double)));
        } else if (rhs.type() == typeid(rational)) {
            const auto &r = dynamic_cast<const rational &>(rhs);
            auto lhs_int = static_cast<int>(number_) * r.denominator();
            auto rhs_int = static_cast<int>(number_) * r.denominator();
            return sym(rational(lhs_int % rhs_int, r.denominator()));
        } else {
            throw sym_error(sym_error::NotNumeric);
        }
    }

    sym integer::zero() { return sym(integer(0)); }

    sym integer::one() { return sym(integer(1)); }
} // namespace sympp