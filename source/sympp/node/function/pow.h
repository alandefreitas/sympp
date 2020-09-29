//
// Created by Alan Freitas on 18/09/20.
//

#ifndef SYMPP_POW_H
#define SYMPP_POW_H

#include <sympp/core/sym.h>
#include <sympp/node/function/function_interface.h>

namespace sympp {
    class product;
    class sin;
    class cos;
    class sinh;
    class cosh;

    class pow : public function_interface {
      public:
        friend class product;
        friend class log;

      public:
        pow(const pow &);
        /// Move constructor
        pow(pow &&) noexcept;
        pow(const sym &, const sym &);
        ~pow() override;

      public /* implement node interface */:
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
      public /* override node interface */:
        std::optional<sym> powdenest() override;
        std::optional<sym> expand() override;
        std::optional<sym> expand_power_exp() override;
        std::optional<sym> expand_power_base() override;
      public /* override internal node interface */:
        [[nodiscard]] node_interface *clone() const override;
      public /* override function interface */:
        std::optional<sym> simplify(double ratio,
                                    complexity_lambda func) override;
        void stream(std::ostream &os, bool symbolic_format) const override;
    };
} // namespace sympp

#endif // SYMPP_POW_H
