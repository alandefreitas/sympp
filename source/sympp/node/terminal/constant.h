//
// Created by thiago on 20/08/19.
//

#ifndef SYMPP_CONSTANT_H
#define SYMPP_CONSTANT_H

// C++
#include <functional>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

// Internal
#include <sympp/core/sym.h>
#include <sympp/core/terminal_node_interface.h>
#include <sympp/node/terminal/number_interface.h>

namespace sympp {

    class constant : public terminal_node_interface<constant> {
      public:
        static sym i();
        static sym e();
        static sym pi();

      public:
        /// Create constant and give it an name
        constant();

        /// Copy constructor
        constant(const constant &);

        /// Move constructor
        constant(constant &&) noexcept;

        /// Constructor from constant name and value
        explicit constant(std::string_view, double d);
        explicit constant(std::string_view, int d);
        explicit constant(std::string_view, bool d);
        explicit constant(std::string_view, const sym &d);

        explicit constant(const node_interface &);

        explicit constant(const sym &);

        ~constant() override;

      public /* node_interface pure-virtual functions */:
        [[nodiscard]] int
        compare(const node_interface &an_interface) const override;

        [[nodiscard]] sym
        coeff(const node_interface &an_interface) const override;

        [[nodiscard]] double
        evaluate(const std::vector<uint8_t> &bool_values,
                 const std::vector<int> &int_values,
                 const std::vector<double> &double_values) const override;

        [[nodiscard]] sym
        evaluate_sym(const std::vector<uint8_t> &bool_values,
                     const std::vector<int> &int_values,
                     const std::vector<double> &double_values) const override;

        [[nodiscard]] node_lambda lambdify() const override;
        void c_code(int &function_level, int &sum_level, int &prod_level,
                    bool &leaf, std::stack<std::string> &code_aux,
                    std::string &code) const override;

      public /* terminal_node_interface virtual functions */:
        void stream(std::ostream &os, bool b) const override;

      public /* getters and setters */:
        /// Convert to the original number
        explicit operator double() const;
        explicit operator int() const;
        explicit operator bool() const;

        /// True of underlying number is zero
        [[nodiscard]] virtual bool is_zero() const;

        /// True if underlying number is one
        [[nodiscard]] virtual bool is_one() const;

        const sym &value() const;
        sym &value();

        const std::string &name() const;
        std::string &name();

      private:
        /// Name we use to stream this constant
        std::string name_;

        /// Number type of this constant when compiling
        sym value_;
    };
} // namespace sympp

#endif // SYMPP_CONSTANT_H
