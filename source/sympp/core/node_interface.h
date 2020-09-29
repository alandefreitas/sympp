//
// Created by Alan de Freitas on 02/08/19.
//

#ifndef SYMPP_NODE_INTERFACE_H
#define SYMPP_NODE_INTERFACE_H

// C++
#include <functional>
#include <iostream>
#include <optional>
#include <stack>
#include <typeinfo>
#include <unordered_map>
#include <vector>

// Internal
#include <sympp/core/sym.h>

namespace sympp {
    // forward declarations
    class sym;

    class statement;

    /// Class with the functions any symbol in the tree should have
    class node_interface {
      public:
        virtual ~node_interface();
      public /* pure virtual functions */:
        /// Stream the node to ostream
        virtual void stream(std::ostream &, bool) const = 0;

        /// Substitute instances of a variable in a expression
        /// Returns an optional sym, in case the node needs to change its type
        virtual std::optional<sym> subs(const sym &, const sym &) = 0;

        /// Compare two symbols for <, ==, >
        /// This is the spaceship operator <=>
        [[nodiscard]] virtual int compare(const node_interface &) const = 0;

        /// Get coefficients
        [[nodiscard]] virtual sym coeff(const node_interface &) const = 0;

        /// True if the operation is commutative
        [[nodiscard]] virtual bool is_commutative() const;

        /// Put indexes in the symbol

        /// Put indexes in the symbol using auxiliary unordered maps
        /// By default, for most nodes, this function does nothing
        /// We need these indexes to compile the variables
        virtual void
        put_indexes(std::unordered_map<int, sym> &bool_symbols_vars,
                    std::unordered_map<int, sym> &int_symbols_vars,
                    std::unordered_map<int, sym> &real_symbols_vars,
                    std::unordered_map<std::string, int> &bool_symbols_names,
                    std::unordered_map<std::string, int> &int_symbols_names,
                    std::unordered_map<std::string, int> &real_symbols_names);

        /// Evaluate expression to a number
        [[nodiscard]] virtual double
        evaluate(const std::vector<uint8_t> &bool_values,
                 const std::vector<int> &int_values,
                 const std::vector<double> &double_values) const = 0;

        /// Evaluate expression to a symbol
        [[nodiscard]] virtual sym
        evaluate_sym(const std::vector<uint8_t> &bool_values,
                     const std::vector<int> &int_values,
                     const std::vector<double> &double_values) const = 0;

        /// Compile expression to a function pointer
        [[nodiscard]] virtual node_lambda lambdify() const = 0;

        /// Compile expression to a string with C code
        virtual void c_code(int &function_level, int &sum_level,
                            int &prod_level, bool &leaf,
                            std::stack<std::string> &code_aux,
                            std::string &code) const = 0;

        /// Number of terms in an expression
        [[nodiscard]] virtual size_t size() const = 0;

        /// Number of operations in the whole expression tree
        [[nodiscard]] virtual size_t count_ops() const = 0;

        /// Create a copy of this node and return a pointer as node_interface
        [[nodiscard]] virtual node_interface *clone() const = 0;

        /// Return true if this is a terminal node
        [[nodiscard]] virtual bool is_terminal() const = 0;

      public /* virtual functions */:
        /// Return type of this symbolic variable
        [[nodiscard]] virtual const std::type_info &type() const;

        /// Simplify expression
        /// \return Returns an optional sym, in case the node needs
        /// to change its type if the node does not need to change
        /// its type to simplify itself this function should return
        /// std::nullopt
        virtual std::optional<sym> simplify(double ratio,
                                            complexity_lambda func);

        /// Simplify expression
        /// You can overload this to call simplify(ratio,fn) with
        /// default options or to apply the operations that
        /// don't depend on fn (the only that can only reduce
        /// the terms)
        virtual std::optional<sym> simplify(double ratio);

        /// Simplify expression
        /// You can overload this to call simplify(ratio) with
        /// default options or to apply the normalization operations
        /// that are obvious for any ratio, like removing zeros from
        /// sums
        virtual std::optional<sym> simplify();

        /// Put polynomial into a canonical form of a sum of monomials
        virtual std::optional<sym> expand();

        /// Takes a polynomial and factors it into irreducible factors
        virtual std::optional<sym> factor();

        /// Collects common powers of a term in an expression
        virtual std::optional<sym> collect();

        /// Take any rational function and put it into the standard canonical
        /// form
        virtual std::optional<sym> cancel();

        /// Performs a partial fraction decomposition on a rational function
        virtual std::optional<sym> appart();

        /// Simplify expressions using trigonometric identities
        virtual std::optional<sym> trigsimp();

        /// Expand trigonometric functions, that is, apply the sum or double
        /// angle identities
        virtual std::optional<sym> expand_trig();

        /// Applies identities x^ax^b=x^{a+b} and x^ay^a=(xy)^a from left to
        /// right Identity x^ay^a=(xy)^a is true if at least x and y are
        /// non-negative and a is real.
        virtual std::optional<sym> powsimp();

        /// Applies identity x^ax^b=x^{a+b} from right to left
        virtual std::optional<sym> expand_power_exp();

        /// Applies identity x^ay^a=(xy)^a from right to left
        /// Identity x^ay^a=(xy)^a is true if at least x and y are non-negative
        /// and a is real.
        virtual std::optional<sym> expand_power_base();

        /// Applies identity x^a^b=x^{ab} from left to right
        /// Identity x^a^b=x^{ab} is true if b is an integer
        virtual std::optional<sym> powdenest();

        /// Apply identities log(xy)=log(x)+log(y) and log(x^n)=nlog(x) from
        /// left to right Only if x and y are positive and n is real.
        virtual std::optional<sym> expand_log();

        /// Apply identities log(xy)=log(x)+log(y) and log(x^n)=nlog(x) from
        /// right to left Only if x and y are positive and n is real.
        virtual std::optional<sym> logcombine();

        /// Iterate the terms of an internal node (null iterator otherwise)
        [[nodiscard]] virtual std::vector<sym>::iterator begin();

        /// Iterate the terms of an internal node (null iterator otherwise)
        [[nodiscard]] virtual std::vector<sym>::iterator end();

        /// Iterate the terms of an internal node (null iterator otherwise)
        [[nodiscard]] virtual std::vector<sym>::const_iterator begin() const;

        /// Iterate the terms of an internal node (null iterator otherwise)
        [[nodiscard]] virtual std::vector<sym>::const_iterator end() const;
    };

} // namespace sympp

#endif // SYMPP_NODE_INTERFACE_H
