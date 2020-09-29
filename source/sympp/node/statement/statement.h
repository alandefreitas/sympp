// equation.h

#ifndef SYMPP_EQUATION
#define SYMPP_EQUATION

#include <sympp/core/internal_node_interface.h>
#include <sympp/core/sym.h>
#include <vector>

namespace sympp {

    class statement : public internal_node_interface<statement> {
      public:
        enum statement_type {
            equality,
            greater_than,
            less_than,
            greater_equal,
            less_equal,
            inequality
        };

      public:
        statement(const statement &);
        statement(const internal_node_interface<statement> &);
        /// Move constructor
        statement(statement &&) noexcept;
        statement(const sym &, const sym &);
        statement(const sym &, const sym &, statement_type);
        statement(sym &&, sym &&, statement_type);
        ~statement() override;
      public /* implement internal node interface */:
        [[nodiscard]] sym coeff(const node_interface &) const override;
      public /* override internal node interface */:
        void stream(std::ostream &os, bool symbolic_format) const override;
        [[nodiscard]] bool is_commutative() const override;
      public /* override internal node interface */:
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

      public:
        [[nodiscard]] const sym &lhs() const;
        sym &lhs();
        [[nodiscard]] const sym &rhs() const;
        sym &rhs();
        explicit operator bool() const;

        statement_type statement_enum_type() const;
        void statement_enum_type(statement_type);

      private:
        statement_type type_{statement_type::equality};
    };

} // namespace sympp

#endif
