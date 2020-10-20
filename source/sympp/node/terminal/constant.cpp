//
// Created by thiago on 20/08/19.
//

#include "constant.h"
#include <stack>
#include <sympp/core/node_interface.h>
#include <sympp/core/sym_error.h>
#include <sympp/node/function/pow.h>
#include <sympp/node/terminal/boolean.h>
#include <sympp/node/terminal/integer.h>
#include <sympp/node/terminal/real.h>

namespace sympp {

    sym constant::i() {
        return sym(constant("i", sym(pow(sym(-1), sym(pow(sym(2), sym(-1)))))));
    }

    sym constant::e() { return sym(constant("e", sym(2.71828))); }

    sym constant::pi() { return sym(constant("pi", sym(3.14159))); }

    constant::constant() = default;

    constant::constant(const constant &) = default;

    constant::constant(constant &&v) noexcept = default;

    constant::constant(std::string_view var_name, double d)
            : name_(var_name), value_(d) {}

    constant::constant(std::string_view var_name, int d)
            : name_(var_name), value_(d) {}

    constant::constant(std::string_view var_name, bool d)
            : name_(var_name), value_(d) {}

    constant::constant(std::string_view var_name, const sym &d)
            : name_(var_name), value_(d) {}

    constant::constant(const node_interface &v)
            : constant(dynamic_cast<const constant &>(v)) {}

    constant::constant(const sym &v) : constant(*v.root_node()) {}

    constant::~constant() = default;

    int constant::compare(const node_interface &s) const {
        if (type() != s.type()) {
            return (type().before(s.type())) ? -1 : +1;
        }
        const auto &rhs = dynamic_cast<const constant &>(s);
        return value_.compare(rhs.value_);
    }

    sym constant::coeff(const node_interface &s) const {
        if (this->compare(s) == 0) {
            return sym(integer(1));
        }
        return sym(integer(0));
    }

    double constant::evaluate(const std::vector<uint8_t> &bool_values,
                              const std::vector<int> &int_values,
                              const std::vector<double> &double_values) const {
        return static_cast<double>(value_.root_node()->evaluate(bool_values, int_values, double_values));
    }

    sym constant::evaluate_sym(const std::vector<uint8_t> &bool_values,
                               const std::vector<int> &int_values,
                               const std::vector<double> &double_values) const {
        return sym(evaluate(bool_values, int_values, double_values));
    }

    node_lambda constant::lambdify() const {
        auto d = this->value_.lambdify();
        return [d](const std::vector<uint8_t> &b, const std::vector<int> &i,
                   const std::vector<double> &dbls) -> double {
            return d(b, i, dbls);
        };
    }

    void constant::c_code(int &, int &, int &, bool &leaf,
                          std::stack<std::string> &code_aux,
                          std::string &code) const {
        auto d = static_cast<double>(this->value_);
        if (leaf) {
            code_aux.push(std::to_string(d));
        } else {
            code += std::to_string(d);
        }
    }

    void constant::stream(std::ostream &os, bool) const { os << name_; }

    constant::operator double() const {
        return static_cast<double>(this->value_);
    }

    constant::operator int() const { return static_cast<int>(this->value_); }

    constant::operator bool() const { return static_cast<bool>(this->value_); }

    const sym &constant::value() const { return value_; }

    sym &constant::value() { return value_; }

    const std::string &constant::name() const { return name_; }

    std::string &constant::name() { return name_; }

    bool constant::is_zero() const {
        if (value().is_number()) {
            auto n = value().root_node_as<number_interface>();
            return n->is_zero();
        }
        throw sym_error(sym_error::NotNumeric);
    }

    bool constant::is_one() const {
        if (value().is_number()) {
            auto n = value().root_node_as<number_interface>();
            return n->is_one();
        }
        throw sym_error(sym_error::NotNumeric);
    }

} // namespace sympp
