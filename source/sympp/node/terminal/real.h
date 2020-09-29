//
// Created by Alan Freitas on 20/09/20.
//

#ifndef SYMPP_REAL_H
#define SYMPP_REAL_H

#include <sympp/node/terminal/number_interface.h>

namespace sympp {
    class real : public number_interface {
      public:
        real();
        real(const real &);
        real(real &&) noexcept;
        explicit real(bool);
        explicit real(int);
        explicit real(double);
        explicit real(const number_interface &);
        explicit real(const node_interface &);
        explicit real(const sym &);
        ~real() override;

      public /* node_interface virtual functions */:
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

        void stream(std::ostream &os, bool b) const override;

        [[nodiscard]] const std::type_info &type() const override;

        [[nodiscard]] node_interface *clone() const override;

      public /* number_interface virtual functions */:
        [[nodiscard]] const std::type_info &numeric_type() const override;

        [[nodiscard]] bool is_zero() const override;

        [[nodiscard]] bool is_one() const override;

        [[nodiscard]] bool is_negative() const override;

        [[nodiscard]] int
        compare_number(const number_interface &an_interface) const override;

        /// Convert the node to bool (might lose precision)
        explicit operator bool() const override;

        /// Convert the node to int (might lose precision)
        explicit operator int() const override;

        /// Convert the node to double (might lose precision)
        explicit operator double() const override;

        [[nodiscard]] sym
        add(const number_interface &an_interface) const override;

        [[nodiscard]] sym
        sub(const number_interface &an_interface) const override;

        [[nodiscard]] sym
        mul(const number_interface &an_interface) const override;

        [[nodiscard]] sym
        div(const number_interface &an_interface) const override;

        [[nodiscard]] sym
        mod(const number_interface &an_interface) const override;

      public /* helpers */:
        static sym zero();

        static sym one();

      private:
        double number_;
    };
} // namespace sympp

#endif // SYMPP_REAL_H
