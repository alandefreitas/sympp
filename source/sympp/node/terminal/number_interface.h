//
// Created by Alan Freitas on 20/09/20.
//

#ifndef SYMPP_NUMBER_INTERFACE_H
#define SYMPP_NUMBER_INTERFACE_H

// C++
#include <cmath>
#include <functional>
#include <iostream>
#include <limits>
#include <type_traits>
#include <typeinfo>
#include <utility>

// Internal
#include <sympp/core/sym.h>
#include <sympp/core/terminal_node_interface.h>

namespace sympp {
    /// \class Traits in common for all number nodes
    /// There is one object for each number type
    /// because each number type has its very
    /// specific logic.
    class number_interface : public terminal_node_interface<number_interface> {
      public:
        ~number_interface() override = default;
      public /* virtual functions */:
        /// Compare two symbols for <, ==, >
        [[nodiscard]] int compare(const node_interface &) const override;

        /// Coefficients
        [[nodiscard]] sym coeff(const node_interface &s) const override;

      public /* virtual functions for numbers */:
        /// Type of the base number
        [[nodiscard]] virtual const std::type_info &numeric_type() const = 0;

        /// True of underlying number is zero
        [[nodiscard]] virtual bool is_zero() const = 0;

        /// True if underlying number is one
        [[nodiscard]] virtual bool is_one() const = 0;

        /// True if underlying number is negative
        [[nodiscard]] virtual bool is_negative() const = 0;

        /// Compare the underlying number type
        /// If the types are different, one of them needs to be promoted
        [[nodiscard]] virtual int
        compare_number(const number_interface &) const = 0;

        /// Convert the node to bool (might lose precision)
        virtual explicit operator bool() const;

        /// Convert the node to int (might lose precision)
        virtual explicit operator int() const;

        /// Convert the node to double (might lose precision)
        virtual explicit operator double() const = 0;

      public /* virtual numeric operators */:
        /// These operators return sym because numbers might change their type
        /// during these operations.

        /// Addition operation
        [[nodiscard]] virtual sym add(const number_interface &) const = 0;

        /// Subtraction operation
        [[nodiscard]] virtual sym sub(const number_interface &) const = 0;

        /// Multiply operation
        [[nodiscard]] virtual sym mul(const number_interface &) const = 0;

        /// Divide operation
        [[nodiscard]] virtual sym div(const number_interface &) const = 0;

        /// Mod operation
        [[nodiscard]] virtual sym mod(const number_interface &) const = 0;

      public /* overloads that forward to the virtual numeric operators */:
        /// Overload operator+ by forwarding to add
        sym operator+(const number_interface &) const;

        /// Overload operator- by forwarding to sub
        sym operator-(const number_interface &) const;

        /// Overload operator* by forwarding to mul
        sym operator*(const number_interface &) const;

        /// Overload operator/ by forwarding to div
        sym operator/(const number_interface &) const;

        /// Overload operator% by forwarding to mod
        sym operator%(const number_interface &) const;

        /// Overload operator== by forwarding to compare_number
        bool operator==(const number_interface &) const;

        /// Overload operator< by forwarding to compare_number
        bool operator<(const number_interface &) const;

        /// Overload operator> by forwarding to compare_number
        bool operator>(const number_interface &) const;

        /// Overload operator<= by forwarding to compare_number
        bool operator<=(const number_interface &) const;

        /// Overload operator>= by forwarding to compare_number
        bool operator>=(const number_interface &) const;
    };

} // namespace sympp
#endif // SYMPP_NUMBER_INTERFACE_H
