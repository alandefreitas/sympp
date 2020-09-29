//
// Created by Alan Freitas on 18/09/20.
//

// C++
#include <cmath>

// Internal
#include "pow.h"
#include <sympp/core/sym.h>
#include <sympp/functions/operators.h>
#include <sympp/functions/symbolic.h>
#include <sympp/node/function/cos.h>
#include <sympp/node/function/cosh.h>
#include <sympp/node/function/log.h>
#include <sympp/node/function/sin.h>
#include <sympp/node/function/sinh.h>
#include <sympp/node/operation/product.h>
#include <sympp/node/terminal/constant.h>
#include <sympp/node/terminal/integer.h>
#include <sympp/node/terminal/number_interface.h>
#include <sympp/node/terminal/real.h>

namespace sympp {

    pow::pow(const pow &s) = default;

    pow::pow(pow &&v) noexcept : function_interface(v) {}

    pow::pow(const sym &b, const sym &e) : function_interface("pow", {b, e}) {}

    pow::~pow() = default;

    double pow::evaluate(const std::vector<uint8_t> &bool_values,
                         const std::vector<int> &int_values,
                         const std::vector<double> &double_values) const {
        if (this->child_nodes_.front().compare(constant::e()) == 0) {
            double e = 2.71828182845;
            auto exponent = this->child_nodes_.back().root_node();
            return std::pow(
                e, exponent->evaluate(bool_values, int_values, double_values));
        } else {
            auto base_node = this->child_nodes_.front().root_node();
            auto exponent_node = this->child_nodes_.back().root_node();
            double base =
                base_node->evaluate(bool_values, int_values, double_values);
            double exponent =
                exponent_node->evaluate(bool_values, int_values, double_values);
            return std::pow(base, exponent);
        }
    }

    sym pow::evaluate_sym(const std::vector<uint8_t> &bool_values,
                          const std::vector<int> &int_values,
                          const std::vector<double> &double_values) const {
        if (this->child_nodes_.front().compare(constant::e()) == 0) {
            return sym(pow(constant::e(),
                           this->child_nodes_.back().root_node()->evaluate_sym(
                               bool_values, int_values, double_values)));
        } else {
            return sym(pow(this->child_nodes_.front().root_node()->evaluate_sym(
                               bool_values, int_values, double_values),
                           this->child_nodes_.back().root_node()->evaluate_sym(
                               bool_values, int_values, double_values)));
        }
    }

    node_lambda pow::lambdify() const {
        node_lambda base_fn;
        node_lambda exponent_fn;

        if (this->child_nodes_.front().compare(constant::e()) == 0) {
            real e_val(2.71828182845);
            base_fn = e_val.lambdify();
        } else {
            base_fn = child_nodes_.front().root_node()->lambdify();
        }

        exponent_fn = this->child_nodes_.back().root_node()->lambdify();

        return [base_fn, exponent_fn](
                   const std::vector<uint8_t> &bool_values,
                   const std::vector<int> &int_values,
                   const std::vector<double> &double_values) -> double {
            return std::pow(
                base_fn(bool_values, int_values, double_values),
                exponent_fn(bool_values, int_values, double_values));
        };
    }

    void pow::c_code(int &function_level, int &sum_level, int &prod_level,
                     bool &leaf, std::stack<std::string> &code_aux,
                     std::string &code) const {
        function_level++;
        if (this->child_nodes_.front().is_summation()) {
            this->child_nodes_.front().root_node()->c_code(
                function_level, sum_level, prod_level, leaf, code_aux, code);
            code += code_aux.top();
            code_aux.pop();
            code += " pow(";
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
            code += " pow(";
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
                code += " pow(";
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
                // Take summation from stack
                code += code_aux.top();
                code_aux.pop();
                code += " pow(";
                code += temp;
            } else if (!code_aux.empty()) {
                code += code_aux.top();
                code_aux.pop();
                code += " pow(";
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
                code += " pow(";
                code += temp;
            } else if (!code_aux.empty()) {
                code += " pow(";
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

    std::optional<sym> pow::simplify(double ratio, complexity_lambda func) {
        sym &b = child_nodes_.front();
        b.simplify(ratio, func);
        sym &n = child_nodes_.back();
        n.simplify(ratio, func);

        // Trivial cases
        if (n.compare(sym(0)) == 0) {
            return sym(integer(1));
        }
        if (n.compare(sym(1)) == 0) {
            return b;
        }

        if (b.compare(sym(0)) == 0) {
            return sym(integer(0));
        }
        if (b.compare(sym(1)) == 0) {
            return sym(integer(1));
        }

        // x^a^b -> x^{a*b}
        powdenest();

        if (b.is_number()) {
            // c^n -> c*c*c*c
            // c^{-n} -> 1/(c*c*c*c)
            if (n.is_integer_number()) {
                int n_as_int = n.root_node_as<integer>()->operator int();
                bool invert = n_as_int < 0;
                auto positive_n =
                    static_cast<unsigned int>(invert ? -n_as_int : n_as_int);
                sym result(integer(1));
                sym prod = b;
                while (positive_n != 0) {
                    if (positive_n & 1U) {
                        result = product(result, prod);
                    }
                    prod = product(prod, prod);
                    positive_n >>= 1U;
                }
                if (invert) {
                    return (sym(integer(1)) / result);
                } else {
                    return result;
                }
            }
        }

        // b^{log_x(b)} -> x
        if (n.is<log>()) {
            if (n.begin()->compare(b) == 0) {
                return *std::prev(n.end());
            }
        }

        if (b.is_number()) {
            // pow(b,n) -> real(std::pow(b,n))
            if (n.is_number()) {
                double bd = b.root_node_as<real>()->operator double();
                double nd = n.operator double();
                if (bd >= 0.0 || int(nd) == nd) {
                    return sym(real(std::pow(bd, nd)));
                }
            }
        }

        return std::nullopt;
    }

    node_interface *pow::clone() const {
        return dynamic_cast<node_interface *>(new pow(*this));
    }

    void pow::stream(std::ostream &os, bool symbolic_format) const {
        if (*this == constant::i()) {
            os << "i";
            return;
        }

        bool parens1 = !(child_nodes_.front().is<variable>()) &&
                       !(child_nodes_.front().is<sin>()) &&
                       !(child_nodes_.front().is<cos>()) &&
                       !(child_nodes_.front().is<sinh>()) &&
                       !(child_nodes_.front().is<cosh>()) &&
                       !(child_nodes_.front().is<log>());

        bool parens2 = !(child_nodes_.back().is<variable>()) &&
                       !(child_nodes_.back().is<sin>()) &&
                       !(child_nodes_.back().is<cos>()) &&
                       !(child_nodes_.back().is<sinh>()) &&
                       !(child_nodes_.back().is<cosh>()) &&
                       !(child_nodes_.back().is<log>());

        // Checking how to stream (Symbolic or C++)
        if (symbolic_format) {
            if (parens1) {
                os << "(";
            }
            child_nodes_.front().stream(os, symbolic_format);
            if (parens1) {
                os << ")";
            }
            os << "^";
            if (parens2) {
                os << "(";
            }
            child_nodes_.back().stream(os, symbolic_format);
            if (parens2) {
                os << ")";
            }
        } else {
            if (child_nodes_.front().compare(constant::e()) == 0) {
                os << "std::exp(";
                child_nodes_.front().stream(os, symbolic_format);
                child_nodes_.back().stream(os, symbolic_format);
                os << ")";
            } else {
                os << "std::pow(";
                child_nodes_.front().stream(os, symbolic_format);
                os << ", ";
                child_nodes_.back().stream(os, symbolic_format);
                os << ")";
            }
        }
    }

    std::optional<sym> pow::powdenest() {
        sym &b = child_nodes_.front();
        const sym &n = child_nodes_.back();
        if (b.is<pow>()) {
            auto base_ptr = b.root_node_as<pow>();
            child_nodes_.back() = child_nodes_.back() * n;
            child_nodes_.front() = base_ptr->child_nodes_.front();
        }
        return std::nullopt;
    }

    std::optional<sym> pow::expand() {
        sym &b = child_nodes_.front();
        b.expand();
        sym &n = child_nodes_.back();
        n.expand();

        bool nothing_to_expand =
            !b.is_summation() && !b.is_product() && !b.is_number();
        if (nothing_to_expand) {
            return std::nullopt;
        }

        // a^(b+c) == a^b a^c  when b and c is_commutative
        auto r1 = expand_power_exp();
        if (r1) {
            return *r1;
        }

        // (a*b)^c == a^c b^c  when a and b is_commutative
        auto r2 = expand_power_base();
        if (r2) {
            return *r2;
        }

        return std::nullopt;
    }

    std::optional<sym> pow::expand_power_exp() {
        // a^(b+c) == a^b a^c  when b and c is_commutative
        const sym &b = child_nodes_.front();
        sym &n = child_nodes_.back();
        if (n.is_summation()) {
            product p;
            for (auto &&t : n) {
                p.child_nodes_.emplace_back(pow(b, t));
            }
            return sym(std::move(p));
        }
        return std::nullopt;
    }

    std::optional<sym> pow::expand_power_base() {
        // (a*b)^c == a^c b^c  when a and b is_commutative
        sym &b = child_nodes_.front();
        const sym &n = child_nodes_.back();
        if (b.is_product()) {
            product r;
            for (auto &&t : b) {
                r.child_nodes_.emplace_back(pow(t, n));
            }
            return sym(std::move(r));
        }
        return std::nullopt;
    }
    const std::type_info &pow::type() const { return typeid(decltype(*this)); }

} // namespace sympp