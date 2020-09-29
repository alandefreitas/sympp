//
// Created by Alan Freitas on 20/09/20.
//

#include "boolean.h"
#include "number_interface.h"

#include <sympp/core/sym_error.h>
#include <sympp/node/operation/product.h>
#include <sympp/node/operation/summation.h>
#include <sympp/node/terminal/integer.h>
#include <sympp/node/terminal/rational.h>
#include <sympp/node/terminal/real.h>

namespace sympp {
    boolean::boolean() : number_(false) {}
    boolean::boolean(const boolean &v) : number_(v.number_) {}
    boolean::boolean(boolean &&v) noexcept : number_(v.number_) {}
    boolean::boolean(bool v) : number_(v) {}
    boolean::boolean(int v) : number_(v != 0) {}
    boolean::boolean(double v) : number_(v != 0.) {}
    boolean::boolean(const number_interface &v)
        : boolean(static_cast<bool>(v)) {}
    boolean::boolean(const node_interface &v)
        : boolean(dynamic_cast<const number_interface &>(v)) {}
    boolean::boolean(const sym &v) : boolean(*v.root_node()) {}
    boolean::~boolean() = default;

    double boolean::evaluate(const std::vector<uint8_t> &,
                             const std::vector<int> &,
                             const std::vector<double> &) const {
        return static_cast<double>(number_);
    }

    sym boolean::evaluate_sym(const std::vector<uint8_t> &,
                              const std::vector<int> &,
                              const std::vector<double> &) const {
        return sym(*this);
    }

    node_lambda boolean::lambdify() const {
        // the lambda version of this returns this boolean as a constant
        auto d = static_cast<double>(this->number_);
        return [d](const std::vector<uint8_t> &, const std::vector<int> &,
                   const std::vector<double> &) -> double { return d; };
    }

    void boolean::c_code(int &, int &, int &, bool &leaf,
                         std::stack<std::string> &code_aux,
                         std::string &code) const {
        auto d = static_cast<double>(this->number_);
        if (leaf) {
            code_aux.push(std::to_string(d));
        } else {
            code += std::to_string(d);
        }
    }

    void boolean::stream(std::ostream &o, bool print_format_symbolic) const {
        if (print_format_symbolic) {
            if (number_) {
                o << "True";
            } else {
                o << "False";
            }
        } else {
            o << (double)number_;
        }
    }

    const std::type_info &boolean::type() const {
        return typeid(decltype(*this));
    }

    node_interface *boolean::clone() const {
        return dynamic_cast<node_interface *>(new boolean(*this));
    }

    const std::type_info &boolean::numeric_type() const { return typeid(bool); }

    bool boolean::is_zero() const { return !number_; }

    bool boolean::is_one() const { return number_; }

    bool boolean::is_negative() const { return false; }

    int boolean::compare_number(const number_interface &rhs) const {
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
            auto lhs_int = static_cast<int>(number_);
            auto rhs_int = static_cast<int>(rhs);
            if (lhs_int < rhs_int) {
                return -1;
            } else if (lhs_int > rhs_int) {
                return +1;
            } else {
                return 0;
            }
        } else if (rhs.type() == typeid(boolean)) {
            auto rhs_bool = static_cast<bool>(rhs);
            if (!number_ && rhs_bool) {
                return -1;
            } else if (number_ && !rhs_bool) {
                return +1;
            } else {
                return 0;
            }
        } else if (rhs.type() == typeid(rational)) {
            const auto &r = dynamic_cast<const rational &>(rhs);
            if (!number_) {
                if (r.numerator() > 0) {
                    return -1;
                } else if (r.numerator() < 0) {
                    return +1;
                } else {
                    return 0;
                }
            } else {
                if (r.numerator() > r.denominator()) {
                    return -1;
                } else if (r.numerator() < r.denominator()) {
                    return +1;
                } else {
                    return 0;
                }
            }
        }
        return 0;
    }

    boolean::operator bool() const { return number_; }

    boolean::operator int() const { return static_cast<int>(number_); }

    boolean::operator double() const { return static_cast<double>(number_); }

    sym boolean::add(const number_interface &rhs) const {
        if (rhs.type() == typeid(boolean)) {
            auto rhs_bool = static_cast<bool>(rhs);
            if (number_ && rhs_bool) {
                return sym(integer(2));
            } else {
                return sym(boolean(number_ | rhs_bool));
            }
        } else {
            // If we have to promote the boolean, we delegate
            // this function to a higher level where it's
            // much easier to treat this.
            // We can only do this for commutative functions.
            return rhs.add(*this);
        }
    }

    sym boolean::sub(const number_interface &rhs) const {
        if (rhs.type() == typeid(boolean)) {
            auto rhs_bool = static_cast<bool>(rhs);
            if (!number_ && rhs_bool) {
                return sym(integer(-1));
            } else {
                return sym(boolean(number_ != rhs_bool));
            }
        } else if (rhs.type() == typeid(integer)) {
            auto lhs_int = static_cast<int>(number_);
            auto rhs_int = static_cast<int>(rhs);
            return sym(integer(lhs_int - rhs_int));
        } else if (rhs.type() == typeid(real)) {
            auto lhs_double = static_cast<double>(number_);
            auto rhs_double = static_cast<double>(rhs);
            return sym(real(lhs_double - rhs_double));
        } else if (rhs.type() == typeid(rational)) {
            const auto &r = dynamic_cast<const rational &>(rhs);
            if (!number_) {
                return sym(rational(-1 * r.numerator(), r.denominator()));
            } else {
                return sym(
                    rational(r.denominator() - r.numerator(), r.denominator()));
            }
        } else {
            return sym(summation(sym(*this), sym(product(sym(-1), sym(rhs)))));
        }
    }

    sym boolean::mul(const number_interface &rhs) const {
        if (rhs.type() == typeid(boolean)) {
            auto rhs_bool = static_cast<bool>(rhs);
            return sym(boolean(number_ && rhs_bool));
        } else {
            // if we have to promote the boolean, we delegate
            // this function to a higher level where it's
            // much easier to treat this
            return rhs.mul(*this);
        }
    }

    sym boolean::div(const number_interface &rhs) const {
        if (rhs.type() == typeid(boolean)) {
            auto rhs_bool = static_cast<bool>(rhs);
            if (!rhs_bool) {
                return sym(integer(std::numeric_limits<int>::infinity()));
            } else {
                return sym(number_);
            }
        } else if (rhs.type() == typeid(integer)) {
            auto lhs_int = static_cast<int>(number_);
            auto rhs_int = static_cast<int>(rhs);
            if (rhs_int != 0 && lhs_int % rhs_int == 0) {
                return sym(integer(lhs_int / rhs_int));
            } else {
                return sym(rational(lhs_int, rhs_int));
            }
        } else if (rhs.type() == typeid(real)) {
            auto lhs_double = static_cast<double>(number_);
            auto rhs_double = static_cast<double>(rhs);
            return sym(real(lhs_double / rhs_double));
        } else if (rhs.type() == typeid(rational)) {
            auto lhs_int = static_cast<int>(number_);
            const auto &r = dynamic_cast<const rational &>(rhs);
            return sym(rational(lhs_int * r.denominator(), r.numerator()));
        } else {
            throw sym_error(sym_error::NotNumeric);
        }
    }

    sym boolean::mod(const number_interface &rhs) const {
        if (rhs.type() == typeid(boolean)) {
            auto rhs_bool = static_cast<bool>(rhs);
            if (!rhs_bool) {
                return sym(rational(static_cast<int>(number_), 0));
            } else {
                return sym(boolean(false));
            }
        } else if (rhs.type() == typeid(integer)) {
            auto lhs_int = static_cast<int>(number_);
            auto rhs_int = static_cast<int>(rhs);
            return sym(integer(lhs_int % rhs_int));
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

    sym boolean::zero() { return sym(boolean(false)); }

    sym boolean::one() { return sym(boolean(true)); }

} // namespace sympp