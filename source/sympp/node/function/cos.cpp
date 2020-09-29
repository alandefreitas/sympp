//
// Created by Alan Freitas on 18/09/20.
//

// C++
#include <cmath>

// Internal
#include "cos.h"
#include <sympp/core/sym.h>
#include <sympp/functions/symbolic.h>
#include <sympp/node/operation/product.h>
#include <sympp/node/terminal/integer.h>
#include <sympp/node/terminal/number_interface.h>
#include <sympp/node/terminal/real.h>

namespace sympp {

    cos::cos(const cos &s) = default;

    cos::cos(cos &&v) noexcept : function_interface(v) {}

    cos::cos(const sym &s) : function_interface("cos", {s}) {}

    cos::~cos() = default;

    double cos::evaluate(const std::vector<uint8_t> &bool_values,
                         const std::vector<int> &int_values,
                         const std::vector<double> &double_values) const {
        return std::cos(this->child_nodes_.front().root_node()->evaluate(
            bool_values, int_values, double_values));
    }

    sym cos::evaluate_sym(const std::vector<uint8_t> &bool_values,
                          const std::vector<int> &int_values,
                          const std::vector<double> &double_values) const {
        sym r = this->child_nodes_.front().root_node()->evaluate_sym(
            bool_values, int_values, double_values);
        r.simplify();
        return sym(cos(r));
    }

    node_lambda cos::lambdify() const {
        auto only_term_fn = child_nodes_.front().lambdify();
        return
            [only_term_fn](const std::vector<uint8_t> &bool_values,
                           const std::vector<int> &int_values,
                           const std::vector<double> &double_values) -> double {
                return std::cos(
                    only_term_fn(bool_values, int_values, double_values));
            };
    }

    void cos::c_code(int &function_level, int &sum_level, int &prod_level,
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
                code += " cos(";
                code += "sum[";
                code += std::to_string(prod_level + 1);
                code += "]";
                code += ");\n";
            } else {
                code_aux.push(" cos(sum[" + std::to_string(prod_level + 1) +
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
                code += " cos(";
                code += "prod[";
                code += std::to_string(prod_level + 1);
                code += "]";
                code += ");\n";
            } else {
                code_aux.push(" cos(prod[" + std::to_string(prod_level + 1) +
                              "]) ");
            }
        } else {
            if (!code_aux.empty()) {
                code += code_aux.top();
                code_aux.pop();
                code += " cos(";
                this->child_nodes_.front().root_node()->c_code(
                    function_level, sum_level, prod_level, leaf, code_aux,
                    code);
                code += ");\n";
            } else {
                code += " cos(";
                this->child_nodes_.front().root_node()->c_code(
                    function_level, sum_level, prod_level, leaf, code_aux,
                    code);
                code += ")";
            }
        }
        function_level--;
    }

    std::optional<sym> cos::simplify(double, complexity_lambda) {
        sym &s = child_nodes_.front();
        s.simplify();
        if (s.is_number()) {
            if (s.is_zero()) {
                return sym(integer(1));
            }
            if (s.is_real_number()) {
                auto p = s.root_node_as<number_interface>();
                return sym(real(std::cos(p->operator double())));
            }
        }
        if (s.is_product()) {
            const std::shared_ptr<product> &p = s.root_node_as<product>();
            auto &f = p->child_nodes_.front();
            if (f.is_number()) {
                if (f.compare(sym(-1)) == 0) {
                    p->child_nodes_.erase(p->child_nodes_.begin());
                }
            }
        }
        return std::nullopt;
    }
    const std::type_info &cos::type() const { return typeid(decltype(*this)); }
    node_interface *cos::clone() const {
        return dynamic_cast<node_interface *>(new cos(*this));
    }

} // namespace sympp