//
// Created by Alan Freitas on 18/09/20.
//

// C++
#include <cmath>

// Internal
#include "sin.h"
#include <sympp/core/sym.h>
#include <sympp/functions/symbolic.h>
#include <sympp/node/operation/product.h>
#include <sympp/node/terminal/integer.h>
#include <sympp/node/terminal/number_interface.h>
#include <sympp/node/terminal/real.h>

namespace sympp {

    sin::sin(const sin &s) = default;

    sin::sin(sin &&v) noexcept : function_interface(v) {}

    sin::sin(const sym &s) : function_interface("sin", {s}) {}

    sin::~sin() = default;

    double sin::evaluate(const std::vector<uint8_t> &bool_values,
                         const std::vector<int> &int_values,
                         const std::vector<double> &double_values) const {

       return std::sin(this->child_nodes_.front().root_node()->evaluate(
            bool_values, int_values, double_values));
    }

    sym sin::evaluate_sym(const std::vector<uint8_t> &bool_values,
                          const std::vector<int> &int_values,
                          const std::vector<double> &double_values) const {
        sym r = this->child_nodes_.front().root_node()->evaluate_sym(
            bool_values, int_values, double_values);
        r.simplify();
        return sym(sin(r));
    }

    node_lambda sin::lambdify() const {
        auto only_term_fn = child_nodes_.front().lambdify();
        return
            [only_term_fn](const std::vector<uint8_t> &bool_values,
                           const std::vector<int> &int_values,
                           const std::vector<double> &double_values) -> double {
                return std::sin(
                    only_term_fn(bool_values, int_values, double_values));
            };
    }

    void sin::c_code(int &function_level, int &sum_level, int &prod_level,
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
                code += " sin(";
                code += "sum[";
                code += std::to_string(prod_level + 1);
                code += "]";
                code += ");\n";
            } else {
                code_aux.push(" sin(sum[" + std::to_string(prod_level + 1) +
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
                code += " sin(";
                code += "prod[";
                code += std::to_string(prod_level + 1);
                code += "]";
                code += ");\n";
            } else {
                code_aux.push(" sin(prod[" + std::to_string(prod_level + 1) +
                              "]) ");
            }
        } else {
            if (!code_aux.empty()) {
                code += code_aux.top();
                code_aux.pop();
                code += " sin(";
                this->child_nodes_.front().root_node()->c_code(
                    function_level, sum_level, prod_level, leaf, code_aux,
                    code);
                code += ");\n";
            } else {
                code += " sin(";
                this->child_nodes_.front().root_node()->c_code(
                    function_level, sum_level, prod_level, leaf, code_aux,
                    code);
                code += ")";
            }
        }
        function_level--;
    }

    std::optional<sym> sin::simplify(double, complexity_lambda) {
        sym &s = child_nodes_.front();
        s.simplify();

        if (s.is_number()) {
            if (s.is_zero()) {
                return sym(integer(0));
            }
            if (s.is_integer_number()) {
                auto p = s.root_node_as<number_interface>();
                return sym(real(std::sin(p->operator int ())));
            }
            if (s.is_real_number()) {
                auto p = s.root_node_as<number_interface>();
                return sym(real(std::sin(p->operator double())));
            }
        }

        if (s.is_product()) {
            std::shared_ptr<product> p = s.root_node_as<product>();
            sym &f = p->child_nodes_.front();
            if (f.is_number()) {
                if (f.compare(sym(-1)) == 0) {
                    p->child_nodes_.erase(p->child_nodes_.begin());
                    product minus_p(std::move(p->child_nodes_));
                    minus_p.child_nodes_.insert(minus_p.child_nodes_.begin(),
                                                sym(-1));
                    return sym(std::move(minus_p));
                }
            }
        }

        return std::nullopt;
    }

    const std::type_info &sin::type() const { return typeid(sin); }

    node_interface *sin::clone() const {
        return dynamic_cast<node_interface *>(new sin(*this));
    }

} // namespace sympp