// C++
#include <algorithm>
#include <string>
#include <vector>

// Internal
#include "function_interface.h"
#include <sympp/node/terminal/variable.h>

namespace sympp {

    size_t function_interface::n_unnamed_functions_{0};

    std::string function_interface::generate_function_name() {
        ++n_unnamed_functions_;
        if (n_unnamed_functions_ < 10) {
            return std::string("fn_") + std::to_string(n_unnamed_functions_);
        } else {
            return std::string("fn_{") + std::to_string(n_unnamed_functions_) +
                   std::string("}");
        }
    }

    function_interface::function_interface()
        : function_interface(generate_function_name()) {}

    function_interface::function_interface(const function_interface &) =
        default;

    function_interface::function_interface(function_interface &&v) noexcept
        : internal_node_interface<function_interface>(v),
          name_(std::move(v.name_)), is_commutative_(v.is_commutative_) {}

    function_interface::function_interface(std::string_view fn_name)
        : function_interface(fn_name, 1) {}

    function_interface::function_interface(std::string_view fn_name, int params)
        : function_interface(fn_name, params, false) {}

    function_interface::function_interface(std::string_view fn_name, int params,
                                           bool commutative)
        : function_interface(fn_name, std::vector<sym>(params), commutative) {}

    function_interface::function_interface(
        std::string_view fn_name, const std::vector<sym> &fn_child_nodes_)
        : function_interface(fn_name, fn_child_nodes_, false) {}

    function_interface::function_interface(
        std::string_view fn_name, const std::vector<sym> &fn_child_nodes_,
        bool commutative)
        : internal_node_interface<function_interface>(fn_child_nodes_),
          name_(fn_name), is_commutative_(commutative) {}

    function_interface::~function_interface() = default;

    sym function_interface::coeff(const node_interface &term) const {
        if (this->compare(term)) {
            return sym(1);
        }
        return sym(0);
    }

    void function_interface::stream(std::ostream &os,
                                    bool symbolic_format) const {
        std::vector<sym>::const_iterator i;
        if (symbolic_format) {
            os << name_;
        } else {
            std::string string_symbol;
            bool excluding_caracters_exp = false;
            bool is_array_variable = false;
            bool first_element = true;
            for (char c : name_) {
                if (isdigit(c)) {
                    is_array_variable = true;
                    if (first_element) {
                        os << "[" << c;
                        first_element = false;
                    } else {
                        if (c == 'e') {
                            excluding_caracters_exp = true;
                        } else {
                            os << c;
                            string_symbol += c;
                        }
                    }
                } else {
                    if (c == 'e') {
                        excluding_caracters_exp = true;
                    } else {
                        os << c;
                        string_symbol += c;
                    }
                }
            }
            if (!excluding_caracters_exp && (string_symbol != "cos") &&
                (string_symbol != "sin") && (string_symbol != "sinh") &&
                (string_symbol != "cosh") && (string_symbol != "abs") &&
                is_array_variable) {
                os << "]";
            }
        }
        if (!child_nodes_.empty()) {
            const auto p = dynamic_cast<const function_interface *>(this);
            const bool is_function = p != nullptr;
            const bool is_variable = type() == typeid(variable);
            os << ((is_function || is_variable) ? "[" : "(");
            child_nodes_.front().stream(os, symbolic_format);
            for (i = ++child_nodes_.begin(); i != child_nodes_.end(); ++i) {
                os << ", ";
                i->stream(os, symbolic_format);
            }
            os << ((is_function || is_variable) ? "]" : ")");
        }
    }

    bool function_interface::is_commutative() const { return is_commutative_; }

    std::optional<sym> function_interface::subs(const sym &x, const int &y) {
        auto r = this->subs(x, sym(y));
        if (r) {
            return *r;
        } else {
            return std::nullopt;
        }
    }

    std::optional<sym> function_interface::subs(const sym &x, const sym &y) {
        return internal_node_interface<function_interface>::subs(x, y);
    }

    std::optional<sym> function_interface::subs(const sym &x, const double &y) {
        auto r = this->subs(x, sym(y));
        if (r) {
            return *r;
        } else {
            return std::nullopt;
        }
    }

    void function_interface::is_commutative(bool v) { is_commutative_ = v; }

    sym function_interface::operator~() const {
        std::shared_ptr<node_interface> n_interface(clone());
        auto n = std::dynamic_pointer_cast<function_interface>(n_interface);
        n->is_commutative(!is_commutative_);
        return sym(std::move(*n));
    }

    const std::string &function_interface::name() const { return name_; }

} // namespace sympp