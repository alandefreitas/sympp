//
// Created by Alan Freitas on 18/09/20.
//

// C++
#include <cmath>

// Internal
#include "cosh.h"
#include <sympp/core/sym.h>
#include <sympp/functions/symbolic.h>
#include <sympp/node/operation/product.h>
#include <sympp/node/terminal/integer.h>
#include <sympp/node/terminal/number_interface.h>
#include <sympp/node/terminal/real.h>

namespace sympp {

    cosh::cosh(const cosh &s) = default;

    cosh::cosh(cosh &&v) noexcept : function_interface(v) {}

    cosh::cosh(const sym &s) : function_interface("cosh", {s}) {}

    cosh::~cosh() = default;

    double cosh::evaluate(const std::vector<uint8_t> &bool_values,
                          const std::vector<int> &int_values,
                          const std::vector<double> &double_values) const {
        return std::cosh(this->child_nodes_.front().root_node()->evaluate(
            bool_values, int_values, double_values));
    }

    sym cosh::evaluate_sym(const std::vector<uint8_t> &bool_values,
                           const std::vector<int> &int_values,
                           const std::vector<double> &double_values) const {
        sym r = this->child_nodes_.front().root_node()->evaluate_sym(
            bool_values, int_values, double_values);
        r.simplify();
        return sym(cosh(r));
    }

    node_lambda cosh::lambdify() const {
        auto only_term_fn = child_nodes_.front().lambdify();
        return
            [only_term_fn](const std::vector<uint8_t> &bool_values,
                           const std::vector<int> &int_values,
                           const std::vector<double> &double_values) -> double {
                return std::cosh(
                    only_term_fn(bool_values, int_values, double_values));
            };
    }

    void cosh::c_code(int &function_level, int &sum_level, int &prod_level,
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
                code += " cosh(";
                code += "sum[";
                code += std::to_string(prod_level + 1);
                code += "]";
                code += ");\n";
            } else {
                code_aux.push(" cosh(sum[" + std::to_string(prod_level + 1) +
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
                code += " cosh(";
                code += "prod[";
                code += std::to_string(prod_level + 1);
                code += "]";
                code += ");\n";
            } else {
                code_aux.push(" cosh(prod[" + std::to_string(prod_level + 1) +
                              "]) ");
            }
        } else {
            if (!code_aux.empty()) {
                code += code_aux.top();
                code_aux.pop();
                code += " cosh(";
                this->child_nodes_.front().root_node()->c_code(
                    function_level, sum_level, prod_level, leaf, code_aux,
                    code);
                code += ");\n";
            } else {
                code += " cosh(";
                this->child_nodes_.front().root_node()->c_code(
                    function_level, sum_level, prod_level, leaf, code_aux,
                    code);
                code += ")";
            }
        }
        function_level--;
    }

    std::optional<sym> cosh::simplify(double, complexity_lambda) {
        sym &s = child_nodes_.front();
        s.simplify();
        if (s.is_number()) {
            if (s.is_zero()) {
                return sym(integer(1));
            }
            if (s.is_real_number()) {
                auto p = s.root_node_as<number_interface>();
                return sym(real(std::cosh(p->operator double())));
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

    node_interface *cosh::clone() const {
        return dynamic_cast<node_interface *>(new cosh(*this));
    }

} // namespace sympp