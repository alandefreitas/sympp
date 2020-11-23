//
// Created by Alan Freitas on 20/09/20.
//

// C++
#include <cmath>
#include <iostream>
#include <limits>
#include <typeinfo>
#include <utility>

// Internal
#include "number_interface.h"
#include <sympp/core/sym_error.h>
#include <sympp/functions/symbolic.h>
#include <sympp/node/terminal/constant.h>
#include <sympp/node/terminal/integer.h>
#include <sympp/node/terminal/real.h>

namespace sympp {

    int number_interface::compare(const node_interface &node) const {

        if (node.type() == typeid(constant)) {
            const auto &cons = dynamic_cast<const constant &>(node);
            return compare_number(real(cons.value()));
        } else {
            const auto &p = dynamic_cast<const number_interface &>(node);
            return compare_number(p);
        }
    }

    sym number_interface::operator+(const number_interface &n) const {
        return this->add(n);
    }

    sym number_interface::operator-(const number_interface &n) const {
        return this->sub(n);
    }

    sym number_interface::operator*(const number_interface &n) const {
        return this->mul(n);
    }

    sym number_interface::operator/(const number_interface &n) const {
        return this->div(n);
    }

    sym number_interface::operator%(const number_interface &n) const {
        return this->mod(n);
    }

    bool number_interface::operator==(const number_interface &n) const {
        return this->compare(n) == 0;
    }

    bool number_interface::operator<(const number_interface &n) const {
        return this->compare(n) == -1;
    }

    bool number_interface::operator>(const number_interface &n) const {
        return this->compare(n) == +1;
    }

    bool number_interface::operator<=(const number_interface &n) const {
        return !(*this > n);
    }

    bool number_interface::operator>=(const number_interface &n) const {
        return !(*this < n);
    }

    /// Coefficients
    sym number_interface::coeff(const node_interface &s) const {
        auto nroot = dynamic_cast<const number_interface *>(&s);
        if (nroot) {
            return this->div(*nroot);
        }
        return sym(integer(0));
    }

    number_interface::operator bool() const { return is_one(); }

    number_interface::operator int() const {
        // this is a placeholder implementation
        // but it's much better to override it
        // we use the double operator defined in the
        // derived object and then convert it to int
        return static_cast<int>(static_cast<double>(*this));
    }

} // namespace sympp