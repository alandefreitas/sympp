//
// Created by thiago on 20/08/19.
//

#ifndef SYMPP_VARIABLE_H
#define SYMPP_VARIABLE_H

// C++
#include <functional>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

// Internal
#include <sympp/core/sym.h>
#include <sympp/core/terminal_node_interface.h>

namespace sympp {

    class variable : public terminal_node_interface<variable> {
      public:
        /// Create variable and give it an name
        variable();

        /// Copy constructor
        variable(const variable &);

        /// Move constructor
        variable(variable &&) noexcept;

        /// Constructor from variable name
        explicit variable(std::string_view);

        /// Construct from number type it represents and name
        explicit variable(numeric_type, std::string_view);

        /// Construct from number type it represents and name
        explicit variable(std::string_view, numeric_type);

        /// Construct from number type and generate new name
        explicit variable(numeric_type);

        explicit variable(const node_interface &);

        explicit variable(const sym &);

        ~variable() override;

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

      public /* node_interface virtual functions */:
        void put_indexes(
            std::unordered_map<int, sym> &bool_symbols_vars,
            std::unordered_map<int, sym> &int_symbols_vars,
            std::unordered_map<int, sym> &real_symbols_vars,
            std::unordered_map<std::string, int> &bool_symbols_names,
            std::unordered_map<std::string, int> &int_symbols_names,
            std::unordered_map<std::string, int> &real_symbols_names) override;

      public /* terminal_node_interface virtual functions */:
        void stream(std::ostream &os, bool b) const override;

      public /* getters and setters */:
        /// Index for compiling
        [[nodiscard]] int index() const;

        /// Number type
        [[nodiscard]] numeric_type num_type() const;

      private:
        /// Number of unnamed variables we created
        /// We can use this to create names for new unnamed variables
        static size_t n_unnamed_variables_;

        /// Generate name for a new variable
        static std::string generate_var_name();

      private:
        /// Name we use to stream this variable
        std::string name_;

        /// Index of this variable when compiling the function
        int index_{0};

        /// Number type of this variable when compiling
        numeric_type num_type_{numeric_type::var_real};
    };
} // namespace sympp

#endif // SYMPP_VARIABLE_H
