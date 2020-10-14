//
// Created by Alan Freitas on 18/09/20.
//

// C++
#include <cmath>

// Internal
#include "log.h"
#include <sympp/functions/mathematics.h>
#include <sympp/functions/symbolic.h>
#include <sympp/node/function/pow.h>
#include <sympp/node/operation/product.h>
#include <sympp/node/terminal/constant.h>
#include <sympp/node/terminal/integer.h>
#include <sympp/node/terminal/real.h>

namespace sympp {

    log::log(const log &s) = default;

    log::log(log &&v) noexcept : function_interface(std::move(v)) {}

    log::log(const sym &x) : log(x, constant::e()) {}

    log::log(const sym &x, const sym &base)
        : function_interface("log", {x, base}) {}

    log::~log() = default;

    double log::evaluate(const std::vector<uint8_t> &bool_values,
                         const std::vector<int> &int_values,
                         const std::vector<double> &double_values) const {

        // if (child_nodes_.back().compare(constant::e()) != 0) {
        // This line was changed, in another form a bad_cast error was presented
        if(child_nodes_.back().begin()!=(constant::e().begin())){
            return std::log(child_nodes_.front().evaluate(
                       bool_values, int_values, double_values)) /
                   std::log(child_nodes_.back().evaluate(
                       bool_values, int_values, double_values));
        } else {
            return std::log(child_nodes_.front().evaluate(
                bool_values, int_values, double_values));
        }
    }

    sym log::evaluate_sym(const std::vector<uint8_t> &bool_values,
                          const std::vector<int> &int_values,
                          const std::vector<double> &double_values) const {
        auto x = child_nodes_.front().root_node()->evaluate_sym(
            bool_values, int_values, double_values);
        auto b = child_nodes_.back().root_node()->evaluate_sym(
            bool_values, int_values, double_values);
        sym r(log(x, b));
        r.simplify();
        return r;
    }

    node_lambda log::lambdify() const {
        node_lambda x_lambda =
            this->child_nodes_.front().root_node()->lambdify();
        if (child_nodes_.back().compare(constant::e()) != 0) {
            node_lambda b_lambda =
                this->child_nodes_.back().root_node()->lambdify();
            return [x_lambda, b_lambda](
                       const std::vector<uint8_t> &bool_values,
                       const std::vector<int> &int_values,
                       const std::vector<double> &double_values) -> double {
                double log =
                    std::log(x_lambda(bool_values, int_values, double_values)) /
                    std::log(b_lambda(bool_values, int_values, double_values));
                return log;
            };
        } else {
            return
                [x_lambda](const std::vector<uint8_t> &bool_values,
                           const std::vector<int> &int_values,
                           const std::vector<double> &double_values) -> double {
                    return std::log(
                        x_lambda(bool_values, int_values, double_values));
                };
        }
    }

    void log::c_code(int &function_level, int &sum_level, int &prod_level,
                     bool &leaf, std::stack<std::string> &code_aux,
                     std::string &code) const {
        function_level++;
        if (this->child_nodes_.front().is_summation()) {
            this->child_nodes_.front().root_node()->c_code(
                function_level, sum_level, prod_level, leaf, code_aux, code);
            code += code_aux.top();
            code_aux.pop();
            code += " log(";
            code += "sum[";
            code += std::to_string(sum_level + 1);
            code += "]";
            code += ",";
            if (this->child_nodes_.back().is_summation()) {
                this->child_nodes_.back().root_node()->c_code(
                    function_level, sum_level, prod_level, leaf, code_aux,
                    code);
                code += code_aux.top();
                code_aux.pop();
                code += "sum[";
                code += std::to_string(sum_level + 1);
                code += "]";
            } else if (this->child_nodes_.back().is_product()) {
                this->child_nodes_.back().root_node()->c_code(
                    function_level, sum_level, prod_level, leaf, code_aux,
                    code);
                code += code_aux.top();
                code_aux.pop();
                code += "prod[";
                code += std::to_string(prod_level + 1);
                code += "]";
            } else {
                this->child_nodes_.back().root_node()->c_code(
                    function_level, sum_level, prod_level, leaf, code_aux,
                    code);
            }
            code += ");\n";
        } else if (this->child_nodes_.front().is_product()) {
            this->child_nodes_.front().root_node()->c_code(
                function_level, sum_level, prod_level, leaf, code_aux, code);
            code += code_aux.top();
            code_aux.pop();
            code += " log(";
            code += "prod[";
            code += std::to_string(prod_level + 1);
            code += "]";
            code += ",";
            if (this->child_nodes_.back().is_summation()) {
                this->child_nodes_.back().root_node()->c_code(
                    function_level, sum_level, prod_level, leaf, code_aux,
                    code);
                code += code_aux.top();
                code_aux.pop();
                code += "sum[";
                code += std::to_string(sum_level + 1);
                code += "]";
            } else if (this->child_nodes_.back().is_product()) {
                this->child_nodes_.back().root_node()->c_code(
                    function_level, sum_level, prod_level, leaf, code_aux,
                    code);
                code += code_aux.top();
                code_aux.pop();
                code += "prod[";
                code += std::to_string(prod_level + 1);
                code += "]";
            } else {
                this->child_nodes_.back().root_node()->c_code(
                    function_level, sum_level, prod_level, leaf, code_aux,
                    code);
            }
            code += ");\n";
        } else if (this->child_nodes_.front().is_variable() ||
                   this->child_nodes_.front().is_number()) {
            if (code_aux.empty()) {
                code += " log(";
                this->child_nodes_.front().root_node()->c_code(
                    function_level, sum_level, prod_level, leaf, code_aux,
                    code);
            } else if (code_aux.size() > 1 && function_level == 1) {
                this->child_nodes_.front().root_node()->c_code(
                    function_level, sum_level, prod_level, leaf, code_aux,
                    code);
                // Get elements from stack
                // Internal function and summation
                std::string temp = code_aux.top();
                code_aux.pop();
                // Take sum from stack
                code += code_aux.top();
                code_aux.pop();
                code += " log(";
                code += temp;
            } else {
                code += code_aux.top();
                code_aux.pop();
                code += " log(";
                this->child_nodes_.front().root_node()->c_code(
                    function_level, sum_level, prod_level, leaf, code_aux,
                    code);
            }
            code += ",";
            if (this->child_nodes_.back().is_summation()) {
                this->child_nodes_.back().root_node()->c_code(
                    function_level, sum_level, prod_level, leaf, code_aux,
                    code);
                code += code_aux.top();
                code_aux.pop();
                code += "sum[";
                code += std::to_string(sum_level + 1);
                code += "]";
            } else if (this->child_nodes_.back().is_product()) {
                this->child_nodes_.back().root_node()->c_code(
                    function_level, sum_level, prod_level, leaf, code_aux,
                    code);
                code += code_aux.top();
                code_aux.pop();
                code += "prod[";
                code += std::to_string(prod_level + 1);
                code += "]";
            } else {
                this->child_nodes_.back().root_node()->c_code(
                    function_level, sum_level, prod_level, leaf, code_aux,
                    code);
            }
            if (!code_aux.empty()) {
                code += ");\n";
            } else {
                if (function_level == 1) {
                    code += ");\n";
                } else {
                    code += ")";
                }
            }
        } else {
            this->child_nodes_.front().root_node()->c_code(
                function_level, sum_level, prod_level, leaf, code_aux, code);
            if (code_aux.size() > 1) {
                // Get elements from stack
                // Internal function and summation
                std::string temp = code_aux.top();
                code_aux.pop();
                // Get summation from stack
                code += code_aux.top();
                code_aux.pop();
                code += " log(";
                code += temp;
            } else if (!code_aux.empty()) {
                code += " log(";
                code += code_aux.top();
                code_aux.pop();
            }
            code += ",";
            if (this->child_nodes_.back().is_summation()) {
                this->child_nodes_.back().root_node()->c_code(
                    function_level, sum_level, prod_level, leaf, code_aux,
                    code);
                code += code_aux.top();
                code_aux.pop();
                code += "sum[";
                code += std::to_string(sum_level + 1);
                code += "]";
            } else if (this->child_nodes_.back().is_product()) {
                this->child_nodes_.back().root_node()->c_code(
                    function_level, sum_level, prod_level, leaf, code_aux,
                    code);
                code += code_aux.top();
                code_aux.pop();
                code += "prod[";
                code += std::to_string(prod_level + 1);
                code += "]";
            } else {
                this->child_nodes_.back().root_node()->c_code(
                    function_level, sum_level, prod_level, leaf, code_aux,
                    code);
            }

            if (function_level == 1) {
                code += ");\n";
            } else {
                code += ")";
            }
        }
        function_level--;
    }

    std::optional<sym> log::simplify(double ratio, complexity_lambda func) {
        // log_a(b)
        sym &a = child_nodes_.back();
        a.simplify(ratio, func);
        sym &b = child_nodes_.front();
        b.simplify(ratio, func);

        // Trivial cases
        if (b.is_number() && b.is_one()) {
            return sym(integer(0));
        }
        if (b.compare(a) == 0) {
            return sym(integer(1));
        }

        // log_a(a^c) -> c
        if (b.type() == typeid(pow)) {
            auto p = b.root_node_as<pow>();
            if (p->child_nodes_.front().compare(a) == 0) {
                return p->child_nodes_.back();
            }
        }

        // log_c(b) -> log(c) * ln(b)^-1
        if (a.is_number() && a.is_real_number() && a.operator double() > 0.0) {
            product p(sym(real(std::log(a.operator double()))),
                      sym(pow(ln(b), sym(-1))));
            auto s = p.simplify(ratio, func);
            return s ? *s : sym(p);
        }

        // log_e(b) -> log(b) * ln(a)^-1
        if (a.compare(constant::e()) == 0 && b.is_number()) {
            product p(sym(real(std::log(b.operator double()))),
                      sym(pow(ln(a), sym(-1))));
            auto s = p.simplify(ratio, func);
            return s ? *s : sym(p);
        }

        // log_a(b) -> log(b) *
        if (a.is_integer_number() && a.operator int() > 0) {
            product p(sym(real(std::log(a.operator double()))),
                      sym(pow(ln(b), sym(-1))));
            auto s = p.simplify(ratio, func);
            return s ? *s : sym(p);
        }

        return std::nullopt;
    }

    const std::type_info &log::type() const { return typeid(log); }

    node_interface *log::clone() const {
        return dynamic_cast<node_interface *>(new log(*this));
    }

    void log::stream(std::ostream &os, bool symbolic_format) const {
        if (child_nodes_.size() == 2 &&
            child_nodes_.back().compare(constant::e()) == 0) {
            os << "ln(";
            child_nodes_.front().stream(os, symbolic_format);
            os << ")";
        } else {
            function_interface::stream(os, symbolic_format);
        }
    }

} // namespace sympp