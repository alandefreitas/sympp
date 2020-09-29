// sum.h

#ifndef SYMPP_SUM
#define SYMPP_SUM

// C++
#include <functional>
#include <vector>

// Internal
#include "product.h"
#include <sympp/core/internal_node_interface.h>

namespace sympp {
    class product;
    class summation : public internal_node_interface<summation> {
      public:
        friend class product;

      public:
        summation();
        summation(const summation &);
        summation(const internal_node_interface<summation> &);
        /// Move constructor
        summation(summation &&) noexcept;
        summation(const sym &, const sym &);
        explicit summation(const std::vector<sym> &summands);
        ~summation() override;
        summation &operator=(const summation &);

      public /* node_interface pure virtual functions */:
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

      public /* node_interface virtual functions */:
        /// Collects common powers of a term in an expression
        std::optional<sym> collect() override;

        /// Put polynomial into a canonical form of a sum of monomials
        std::optional<sym> expand() override;

      public /* internal node virtual functions */:
        void stream(std::ostream &os, bool symbolic_format) const override;
        std::optional<sym> simplify(double ratio,
                                    complexity_lambda func) override;

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
        [[nodiscard]] node_interface *clone() const override;

      public:
        /// Absorb sum of sums: a + (a + a) + a -> a + a + a + a
        void absorb_sum_of_sums();
        [[nodiscard]] bool children_match(const summation &rhs) const;
    };

} // namespace sympp

#endif
