//
// Created by Alan Freitas on 20/09/20.
//

#ifndef SYMPP_RATIONAL_H
#define SYMPP_RATIONAL_H

// Internal
#include <sympp/node/terminal/number_interface.h>

namespace sympp {
    class rational : public number_interface {
      public:
        rational();
        rational(const rational &);
        rational &operator=(const rational &);
        rational(rational &&) noexcept;
        rational &operator=(rational &&);
        explicit rational(bool);
        explicit rational(int);
        rational(int, int);
        explicit rational(double);
        explicit rational(const number_interface &);
        explicit rational(const node_interface &);
        explicit rational(const sym &);
        ~rational() override;

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

        std::optional<sym> simplify(double ratio,
                                    complexity_lambda func) override;

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
        /// numerator of r
        [[nodiscard]] int numerator() const;

        /// denominator of r
        [[nodiscard]] int denominator() const;

        /// fractional part of r
        [[nodiscard]] rational frac() const;

        /// normalize the rational number
        void normalize();

        static sym zero();

        static sym one();

      private:
        // Data Fields : Numerator and Denominator
        int numerator_;
        int denominator_;
    };

} // namespace sympp

#endif // SYMPP_RATIONAL_H
