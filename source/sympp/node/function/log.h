//
// Created by Alan Freitas on 18/09/20.
//

#ifndef SYMPP_LOG_H
#define SYMPP_LOG_H

#include <sympp/core/sym.h>
#include <sympp/node/function/function_interface.h>

namespace sympp {
    class product;
    class log : public function_interface {
      public:
        friend class product;

      public:
        log(const log &);
        /// Move constructor
        log(log &&) noexcept;
        explicit log(const sym &);
        log(const sym &, const sym &);
        ~log() override;

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
      public /* override internal node interface */:
        [[nodiscard]] const std::type_info &type() const override;
        [[nodiscard]] node_interface *clone() const override;
      public /* override function interface */:
        std::optional<sym> simplify(double ratio,
                                    complexity_lambda func) override;
        void stream(std::ostream &os, bool symbolic_format) const override;
    };
} // namespace sympp

#endif // SYMPP_LOG_H
