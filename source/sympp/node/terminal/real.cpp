//
// Created by Alan Freitas on 20/09/20.
//

#include "real.h"
#include "number_interface.h"

#include <sympp/core/sym_error.h>
#include <sympp/node/operation/product.h>
#include <sympp/node/operation/summation.h>
#include <sympp/node/terminal/boolean.h>
#include <sympp/node/terminal/integer.h>
#include <sympp/node/terminal/rational.h>

namespace sympp {
    real::real() : number_(0) {}
    real::real(const real &v) : number_(v.number_) {}
    real::real(real &&v) noexcept : number_(v.number_) {}
    real::real(bool v) : number_(static_cast<double>(v)) {}
    real::real(int v) : number_(static_cast<double>(v)) {}
    real::real(double v) : number_(v) {}
    real::real(const number_interface &v) : real(static_cast<double>(v)) {}
    real::real(const node_interface &v)
        : real(dynamic_cast<const number_interface &>(v)) {}
    real::real(const sym &v) : real(*v.root_node()) {}
    real::~real() = default;

    double real::evaluate(const std::vector<uint8_t> &,
                          const std::vector<int> &,
                          const std::vector<double> &) const {
        return number_;
    }

    sym real::evaluate_sym(const std::vector<uint8_t> &,
                           const std::vector<int> &,
                           const std::vector<double> &) const {
        return sym(*this);
    }

    node_lambda real::lambdify() const {
        // the lambda version of this returns this integer as a constant
        double d = this->number_;
        return [d](const std::vector<uint8_t> &, const std::vector<int> &,
                   const std::vector<double> &) -> double { return d; };
    }

    void real::c_code(int &, int &, int &, bool &leaf,
                      std::stack<std::string> &code_aux,
                      std::string &code) const {
        double d = this->number_;
        if (leaf) {
            code_aux.push(std::to_string(d));
        } else {
            code += std::to_string(d);
        }
    }

    void real::stream(std::ostream &o, bool) const { o << number_; }

    const std::type_info &real::type() const { return typeid(decltype(*this)); }

    node_interface *real::clone() const {
        return dynamic_cast<node_interface *>(new real(*this));
    }

    const std::type_info &real::numeric_type() const { return typeid(double); }

    bool real::is_zero() const { return number_ == 0.; }

    bool real::is_one() const { return number_ == 1.; }

    bool real::is_negative() const { return number_ < 0.; }

    int real::compare_number(const number_interface &rhs) const {
        if (rhs.type() == typeid(real) || rhs.type() == typeid(integer)) {
            auto rhs_double = static_cast<double>(rhs);
            if (number_ < rhs_double) {
                return -1;
            } else if (number_ > rhs_double) {
                return +1;
            } else {
                return 0;
            }
        } else if (rhs.type() == typeid(boolean)) {
            auto rhs_bool = static_cast<bool>(rhs);
            if (!rhs_bool) {
                if (number_ < 0.) {
                    return -1;
                } else if (number_ > 0.) {
                    return +1;
                } else {
                    return 0;
                }
            } else {
                if (number_ < 1.) {
                    return -1;
                } else if (number_ > 1.) {
                    return +1;
                } else {
                    return 0;
                }
            }
        } else if (rhs.type() == typeid(rational)) {
            const auto &r = dynamic_cast<const rational &>(rhs);
            double lhs_int = number_ * r.denominator();
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

    real::operator bool() const { return number_ != 0.; }

    real::operator int() const { return static_cast<int>(number_); }

    real::operator double() const { return number_; }

    sym real::add(const number_interface &rhs) const {
        if (rhs.type() == typeid(boolean) || rhs.type() == typeid(integer) ||
            rhs.type() == typeid(real)) {
            // might promote the boolean
            auto rhs_double = static_cast<double>(rhs);
            return sym(real(number_ + rhs_double));
        } else {
            // Delegate commutative function to higher level
            return rhs.add(*this);
        }
    }

    sym real::sub(const number_interface &rhs) const {
        if (rhs.type() == typeid(boolean)) {
            auto rhs_bool = static_cast<bool>(rhs);
            if (rhs_bool) {
                return sym(real(number_ - 1.));
            } else {
                return sym(real(number_));
            }
        } else if (rhs.type() == typeid(integer) ||
                   rhs.type() == typeid(real) ||
                   rhs.type() == typeid(rational)) {
            auto rhs_int = static_cast<double>(rhs);
            return sym(real(number_ - rhs_int));
        } else {
            return sym(summation(sym(*this), sym(product(sym(-1), sym(rhs)))));
        }
    }

    sym real::mul(const number_interface &rhs) const {
        if (rhs.type() == typeid(boolean) || rhs.type() == typeid(integer) ||
            rhs.type() == typeid(rational) || rhs.type() == typeid(real)) {
            auto rhs_double = static_cast<double>(rhs);
            return sym(real(number_ * rhs_double));
        } else {
            throw sym_error(sym_error::NotNumeric);
        }
    }

    sym real::div(const number_interface &rhs) const {
        if (rhs.type() == typeid(boolean) || rhs.type() == typeid(integer) ||
            rhs.type() == typeid(rational) || rhs.type() == typeid(real)) {
            auto rhs_double = static_cast<double>(rhs);
            return sym(real(number_ / rhs_double));
        } else {
            throw sym_error(sym_error::NotNumeric);
        }
    }

    sym real::mod(const number_interface &rhs) const {
        if (rhs.type() == typeid(boolean) || rhs.type() == typeid(integer) ||
            rhs.type() == typeid(rational) || rhs.type() == typeid(real)) {
            auto rhs_double = static_cast<double>(rhs);
            return sym(real(std::fmod(number_, rhs_double)));
        } else {
            throw sym_error(sym_error::NotNumeric);
        }
    }

    sym real::zero() { return sym(real(0.)); }

    sym real::one() { return sym(real(1.)); }
} // namespace sympp