//
// Created by thiago on 20/08/19.
//

#include "variable.h"
#include <stack>
#include <sympp/core/node_interface.h>
#include <sympp/node/terminal/boolean.h>
#include <sympp/node/terminal/integer.h>
#include <sympp/node/terminal/real.h>

namespace sympp {

    size_t variable::n_unnamed_variables_{0};

    std::string variable::generate_var_name() {
        return std::string("x_{") + std::to_string(++n_unnamed_variables_) +
               std::string("}");
    }

    variable::variable() : variable(generate_var_name()) {}

    variable::variable(const variable &) = default;

    variable::variable(variable &&v) noexcept = default;

    variable::variable(std::string_view var_name)
        : variable(numeric_type::var_real, var_name) {}

    variable::variable(numeric_type v, std::string_view var_name)
        : name_(var_name), index_(0), num_type_(v) {}

    variable::variable(std::string_view var_name, numeric_type v)
        : name_(var_name), index_(0), num_type_(v) {}

    variable::variable(numeric_type v)
        : name_(generate_var_name()), index_(0), num_type_(v) {}

    variable::variable(const node_interface &v)
        : variable(dynamic_cast<const variable &>(v)) {}

    variable::variable(const sym &v) : variable(*v.root_node()) {}

    variable::~variable() = default;

    int variable::compare(const node_interface &s) const {
        if (type() != s.type()) {
            return (type().before(s.type())) ? -1 : +1;
        }
        const auto &rhs = dynamic_cast<const variable &>(s);
        if (num_type_ != rhs.num_type_) {
            return (num_type_ < rhs.num_type_) ? -1 : +1;
        } else if (name_ != rhs.name_) {
            return (name_ < rhs.name_) ? -1 : +1;
        } else if (index_ != rhs.index_) {
            return (index_ < rhs.index_) ? -1 : +1;
        }
        return 0;
    }

    sym variable::coeff(const node_interface &s) const {
        if (this->compare(s) == 0) {
            return sym(integer(1));
        }
        return sym(integer(0));
    }

    double variable::evaluate(const std::vector<uint8_t> &bool_values,
                              const std::vector<int> &int_values,
                              const std::vector<double> &double_values) const {
        switch (num_type_) {
        case numeric_type::var_boolean:
            return static_cast<double>(bool_values[this->index_]);
        case numeric_type::var_integer:
            return static_cast<double>(int_values[this->index_]);
        case numeric_type::var_real:
            return double_values[this->index_];
        default:
            return 0;
        }
    }

    sym variable::evaluate_sym(const std::vector<uint8_t> &bool_values,
                               const std::vector<int> &int_values,
                               const std::vector<double> &double_values) const {
        sym this_copy(*this);
        sym this_sym(*this);
        switch (num_type_) {
        case numeric_type::var_boolean:
            this_copy.subs(this_sym, sym(boolean(bool_values[this->index_])));
            break;
        case numeric_type::var_integer:
            this_copy.subs(this_sym, sym(integer(int_values[this->index_])));
            break;
        case numeric_type::var_real:
        default:
            this_copy.subs(this_sym, sym(real(double_values[this->index_])));
            break;
        }
        return this_copy;
    }

    node_lambda variable::lambdify() const {
        switch (num_type_) {
        case numeric_type::var_boolean: {
            auto idx = this->index_;
            return
                [idx](const std::vector<uint8_t> &bool_values,
                      const std::vector<int> &, const std::vector<double> &) {
                    return static_cast<double>(bool_values[idx]);
                };
        }
        case numeric_type::var_integer: {
            auto idx = this->index_;
            return [idx](const std::vector<uint8_t> &,
                         const std::vector<int> &int_values,
                         const std::vector<double> &) {
                return static_cast<double>(int_values[idx]);
            };
        }
        case numeric_type::var_real: {
            auto idx = this->index_;
            return [idx](const std::vector<uint8_t> &, const std::vector<int> &,
                         const std::vector<double> &double_values) {
                return static_cast<double>(double_values[idx]);
            };
        }
        default:
            throw std::runtime_error("Invalid variable type. Cannot lambdify.");
        }
    }

    void variable::c_code(int &, int &, int &, bool &leaf,
                          std::stack<std::string> &code_aux,
                          std::string &code) const {
        switch (num_type_) {
        case numeric_type::var_boolean: {
            if (leaf) {
                code_aux.push("bool_values[" + std::to_string(this->index_) +
                              "]");
            } else {
                code += "bool_values[";
                code += std::to_string(this->index_);
                code += "]";
            }
        }
        case numeric_type::var_integer: {
            if (leaf) {
                code_aux.push("int_values[" + std::to_string(this->index_) +
                              "]");
            } else {
                code += "int_values[";
                code += std::to_string(this->index_);
                code += "]";
            }
        }
        case numeric_type::var_real: {
            if (leaf) {
                code_aux.push("double_values[" + std::to_string(this->index_) +
                              "]");
            } else {
                code += "double_values[";
                code += std::to_string(this->index_);
                code += "]";
            }
        }
        }
    }

    void variable::put_indexes(
        std::unordered_map<int, sym> &bool_symbols_vars,
        std::unordered_map<int, sym> &int_symbols_vars,
        std::unordered_map<int, sym> &real_symbols_vars,
        std::unordered_map<std::string, int> &bool_symbols_names,
        std::unordered_map<std::string, int> &int_symbols_names,
        std::unordered_map<std::string, int> &real_symbols_names) {

        std::unordered_map<int, sym> *symbols_vars = nullptr;
        std::unordered_map<std::string, int> *symbols_names = nullptr;
        switch (num_type_) {
        case var_integer:
            symbols_vars = &int_symbols_vars;
            symbols_names = &int_symbols_names;
            break;
        case var_boolean:
            symbols_vars = &bool_symbols_vars;
            symbols_names = &bool_symbols_names;
            break;
        case var_real:
            symbols_vars = &real_symbols_vars;
            symbols_names = &real_symbols_names;
            break;
        }
        if (symbols_names->empty()) {
            symbols_names->operator[](this->name_) = 0;
            symbols_vars->operator[](0) = sym(*this);
            this->index_ = 0;
        } else {
            // Look for index
            auto search_name = symbols_names->find(this->name_);
            // If not found, add new variable and index
            if (search_name == symbols_names->end()) {

                int cur_index = symbols_names->size();
                symbols_names->operator[](this->name_) = cur_index;
                symbols_vars->operator[](cur_index) = (sym) * this;
                this->index_ = cur_index;

            } else {
                // If found, update symbol index
                this->index_ = search_name->second;
            }
        }
    }

    void variable::stream(std::ostream &os, bool) const { os << name_; }

    int variable::index() const { return index_; }

    numeric_type variable::num_type() const { return num_type_; }

} // namespace sympp
