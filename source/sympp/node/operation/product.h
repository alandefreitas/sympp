// product.h

#ifndef SYMPP_PRODUCT
#define SYMPP_PRODUCT

// C++
#include <algorithm>
#include <vector>

// Internal
#include <sympp/core/internal_node_interface.h>

namespace sympp {
    // Forward-declaration
    class summation;

    class product : public internal_node_interface<product> {
      public:
        friend class summation;
        friend class cos;
        friend class cosh;
        friend class sin;
        friend class sinh;
        friend class pow;

      public:
        product();
        product(const product &);
        product(const internal_node_interface<product> &);
        /// Move constructor
        product(product &&) noexcept;
        product(const sym &, const sym &);
        explicit product(const std::vector<sym> &child_nodes);
        explicit product(std::vector<sym> &&child_nodes);
        ~product() override;
        product &operator=(const product &);

      public:
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

      public:
        std::optional<sym> expand() override;

        std::optional<sym> collect() override;

      public:
        void stream(std::ostream &os, bool symbolic_format) const override;

        std::optional<sym> simplify(double ratio,
                                    complexity_lambda func) override;

        std::optional<sym> simplify(double ratio) override;

        std::optional<sym> simplify() override;

        std::optional<sym> subs(const sym &x, const sym &y) override;

        [[nodiscard]] int compare(const node_interface &s) const override;

        void put_indexes(
            std::unordered_map<int, sym> &bool_symbols_vars,
            std::unordered_map<int, sym> &int_symbols_vars,
            std::unordered_map<int, sym> &real_symbols_vars,
            std::unordered_map<std::string, int> &bool_symbols_names,
            std::unordered_map<std::string, int> &int_symbols_names,
            std::unordered_map<std::string, int> &real_symbols_names) override;

        [[nodiscard]] bool is_commutative() const override;

      public:
        [[nodiscard]] int prints_negative() const;
        [[nodiscard]] bool children_match(const product &rhs) const;
        // expand positive integer powers to match each factor individually
        void expand_powers();
    };
} // namespace sympp

#endif
