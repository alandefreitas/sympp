//
// Created by Alan Freitas on 18/09/20.
//

// C++
#include <cmath>

// Internal
#include "abs.h"
#include <sympp/core/sym.h>
#include <sympp/functions/operators.h>
#include <sympp/functions/symbolic.h>
#include <sympp/node/terminal/integer.h>
#include <sympp/node/terminal/number_interface.h>
#include <sympp/node/terminal/rational.h>

namespace sympp {

    abs::abs(const abs &s) = default;

    abs::abs(abs &&v) noexcept : function_interface(v) {}

    abs::abs(const sym &s) : function_interface("abs", {s}) {}

    abs::~abs() = default;

    double abs::evaluate(const std::vector<uint8_t> &bool_values,
                         const std::vector<int> &int_values,
                         const std::vector<double> &double_values) const {

        return std::abs(this->child_nodes_.front().root_node()->evaluate(
            bool_values, int_values, double_values));
    }

    sym abs::evaluate_sym(const std::vector<uint8_t> &bool_values,
                          const std::vector<int> &int_values,
                          const std::vector<double> &double_values) const {

        auto ce = this->child_nodes_.front().root_node()->evaluate_sym(
            bool_values, int_values, double_values);

        if (ce.is_number()) {

            auto p = ce.root_node_as<number_interface>();

            if (ce.is_integer_number()) {

                if (p->operator int() < 0) {
                    return sym(-1 * (p->operator int()));
                } else {
                    return sym(p->operator int());
                }
            }

            if (ce.is_boolean_number()) {
                return sym(p->operator bool());
            }

            if (ce.is_rational_number()) {

                auto rat = ce.root_node_as<rational>();
                if (rat->numerator() < 0 || rat->denominator() < 0) {
                    return sym(rat->mul(integer(-1)).simplify());
                } else {
                    return sym(rat->mul(integer(1)).simplify());
                }
            }

            if (ce.is_real_number()) {

                if (static_cast<double>(p->operator double()) < 0) {
                    return sym(-1 * (p->operator double()));
                } else {
                    return sym(p->operator double());
                }
            }

            std::cout << " rat2 " << std::endl;

            return sym(abs(ce));

        } else {
            std::cout << " rat " << std::endl;
            return sym(abs(ce));
        }
    }

    node_lambda abs::lambdify() const {
        auto only_term_fn = child_nodes_.front().lambdify();
        return
            [only_term_fn](const std::vector<uint8_t> &bool_values,
                           const std::vector<int> &int_values,
                           const std::vector<double> &double_values) -> double {
                return std::abs(
                    only_term_fn(bool_values, int_values, double_values));
            };
    }

    void abs::c_code(int &function_level, int &sum_level, int &prod_level,
                     bool &leaf, std::stack<std::string> &code_aux,
                     std::string &code) const {
        function_level++;
        if (this->child_nodes_.front().is_summation()) {
            this->child_nodes_.front().root_node()->c_code(
                function_level, sum_level, prod_level, leaf, code_aux, code);
            if (function_level == 1) {
                if (!code_aux.empty()) {
                    code += code_aux.top();
                    code_aux.pop();
                }
                code += " abs(";
                code += "sum[";
                code += std::to_string(prod_level + 1);
                code += "]";
                code += ");\n";
            } else {
                code_aux.push(" abs(sum[" + std::to_string(prod_level + 1) +
                              "]) ");
            }
        } else if (this->child_nodes_.front().is_product()) {
            this->child_nodes_.front().root_node()->c_code(
                function_level, sum_level, prod_level, leaf, code_aux, code);
            if (function_level == 1) {
                if (!code_aux.empty()) {
                    code += code_aux.top();
                    code_aux.pop();
                }
                code += " abs(";
                code += "prod[";
                code += std::to_string(prod_level + 1);
                code += "]";
                code += ");\n";
            } else {
                code_aux.push(" abs(prod[" + std::to_string(prod_level + 1) +
                              "]) ");
            }
        } else if (this->child_nodes_.front().is_variable() ||
                   this->child_nodes_.front().is_number()) {
            if (function_level == 1) {
                if (!code_aux.empty()) {
                    code += code_aux.top();
                    code_aux.pop();
                }
                code += " abs(";
                this->child_nodes_.front().root_node()->c_code(
                    function_level, sum_level, prod_level, leaf, code_aux,
                    code);
                code += ");\n";
            } else {
                leaf = true;
                this->child_nodes_.front().root_node()->c_code(
                    function_level, sum_level, prod_level, leaf, code_aux,
                    code);
                std::string temp = code_aux.top();
                code_aux.pop();

                code_aux.push(" abs(" + temp + ")");

                leaf = false;
            }
        }
        function_level--;
    }

    std::optional<sym> abs::simplify(double ratio, complexity_lambda func) {
        auto &s = child_nodes_.front();
        s.simplify(ratio, func);
        if (s.is_zero()) {
            return sym(integer(0));
        }

        if (s.is_number()) {
            auto p = s.root_node_as<number_interface>();
            if (p->is_negative()) {
                return -1 * s;
            } else {
                return s;
            }
        }
        return std::nullopt;
    }
    const std::type_info &abs::type() const { return typeid(decltype(*this)); }
    node_interface *abs::clone() const {
        return dynamic_cast<node_interface *>(new abs(*this));
    }

} // namespace sympp