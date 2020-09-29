#include <vector>

#include "statement.h"
#include <sympp/core/sym.h>
#include <sympp/functions/operators.h>
#include <sympp/node/function/abs.h>

namespace sympp {

    statement::statement(const statement &s)
        : internal_node_interface<statement>(s.child_nodes_), type_(s.type_) {}

    statement::statement(const internal_node_interface<statement> &s)
        : internal_node_interface<statement>(s) {
        auto srhs = dynamic_cast<const statement *>(&s);
        if (srhs) {
            type_ = srhs->type_;
        } else {
            throw sym_error(sym_error::AbstractClass);
        }
    }

    statement::statement(statement &&v) noexcept
        : internal_node_interface<statement>(v) {}

    statement::statement(const sym &s1, const sym &s2)
        : statement(s1, s2, statement_type::equality) {}

    statement::statement(const sym &s1, const sym &s2, const statement_type t)
        : internal_node_interface<statement>({s1, s2}), type_(t) {}

    statement::statement(sym &&s1, sym &&s2, const statement_type t)
        : internal_node_interface<statement>(std::vector<sym>{s1, s2}),
          type_(t) {}

    statement::~statement() = default;

    sym statement::coeff(const node_interface &) const { return sym(0); }

    void statement::stream(std::ostream &os, bool symbolic_format) const {
        if (symbolic_format) {
            switch (type_) {
            case statement_type::equality:
                os << lhs() << " == " << rhs();
                break;
            case statement_type::greater_than:
                os << lhs() << " > " << rhs();
                break;
            case statement_type::greater_equal:
                os << lhs() << " >= " << rhs();
                break;
            case statement_type::less_than:
                os << lhs() << " < " << rhs();
                break;
            case statement_type::less_equal:
                os << lhs() << " <= " << rhs();
                break;
            case statement_type::inequality:
                os << lhs() << " != " << rhs();
                break;
            }
        } else {
            switch (type_) {
            case statement_type::equality:
                lhs().stream(os, false);
                os << " - ";
                rhs().stream(os, false);
                os << " <= 0 ";
                break;

            case statement_type::greater_than:
                lhs().stream(os, false);
                os << " > ";
                rhs().stream(os, false);
                break;
            case statement_type::greater_equal:
                lhs().stream(os, false);
                os << " >= ";
                rhs().stream(os, false);
                break;
            case statement_type::less_than:
                lhs().stream(os, false);
                os << " < ";
                rhs().stream(os, false);
                break;
            case statement_type::less_equal:
                lhs().stream(os, false);
                os << " <= ";
                rhs().stream(os, false);
                break;
            case statement_type::inequality:
                lhs().stream(os, false);
                os << " != ";
                rhs().stream(os, false);
                break;
            }
        }
    }
    bool statement::is_commutative() const {
        return type_ == statement_type::equality ||
               type_ == statement_type::inequality;
    }

    statement::operator bool() const {
        switch (type_) {
        case statement_type::equality:
            return lhs().compare(rhs()) == 0;
        case statement_type::greater_than:
            return lhs().compare(rhs()) == 1;
        case statement_type::greater_equal:
            return lhs().compare(rhs()) >= 0;
        case statement_type::less_than:
            return lhs().compare(rhs()) == -1;
        case statement_type::less_equal:
            return lhs().compare(rhs()) <= 0;
        case statement_type::inequality:
            return lhs().compare(rhs()) != 0;
        default:
            throw std::runtime_error("Invalid statement type");
        }
    }

    double statement::evaluate(const std::vector<uint8_t> &bool_values,
                               const std::vector<int> &int_values,
                               const std::vector<double> &double_values) const {
        // Evaluate for statements is a special function
        // Instead of returning true or false, it returns not only
        // if the statement is true but also the distance from one equation
        // to the other (i.e.: how far it is to be true).
        // You can later generate a bool with (evaluate(...) >= 0.) if you
        // just want to make sure it is true.
        // Or you can use this distance to know by how much a constraint is
        // broken. This is especially necessary in optimization algorithms.
        double l =
            lhs().root_node()->evaluate(bool_values, int_values, double_values);
        double r =
            rhs().root_node()->evaluate(bool_values, int_values, double_values);
        switch (type_) {
        case statement_type::equality:
            return std::abs(l - r);
        case statement_type::greater_than:
        case statement_type::greater_equal:
            return l - r;
        case statement_type::less_than:
        case statement_type::less_equal:
            return r - l;
        case statement_type::inequality:
            return -std::abs(l - r);
        default:
            throw std::runtime_error("Invalid statement type");
        }
    }

    sym
    statement::evaluate_sym(const std::vector<uint8_t> &bool_values,
                            const std::vector<int> &int_values,
                            const std::vector<double> &double_values) const {
        sym l = lhs().root_node()->evaluate_sym(bool_values, int_values,
                                                double_values);
        sym r = rhs().root_node()->evaluate_sym(bool_values, int_values,
                                                double_values);
        switch (type_) {
        case statement_type::equality: {
            auto s = sym(abs(l - r));
            s.simplify();
            return s;
        }
        case statement_type::greater_than:
        case statement_type::greater_equal:
            return l - r;
        case statement_type::less_than:
        case statement_type::less_equal:
            return r - l;
        case statement_type::inequality: {
            auto s = -sym(abs(l - r));
            s.simplify();
            return s;
        }
        default:
            throw std::runtime_error("Invalid statement type");
        }
    }

    node_lambda statement::lambdify() const {
        node_lambda l = lhs().lambdify();
        node_lambda r = rhs().lambdify();
        switch (type_) {
        case statement_type::equality:
            return [l, r](const std::vector<uint8_t> &bool_values,
                          const std::vector<int> &int_values,
                          const std::vector<double> &double_values) -> double {
                return std::abs(l(bool_values, int_values, double_values) -
                                r(bool_values, int_values, double_values));
            };
        case statement_type::greater_than:
        case statement_type::greater_equal:
            return [l, r](const std::vector<uint8_t> &bool_values,
                          const std::vector<int> &int_values,
                          const std::vector<double> &double_values) -> double {
                return l(bool_values, int_values, double_values) -
                       r(bool_values, int_values, double_values);
            };
        case statement_type::less_than:
        case statement_type::less_equal:
            return [l, r](const std::vector<uint8_t> &bool_values,
                          const std::vector<int> &int_values,
                          const std::vector<double> &double_values) -> double {
                return r(bool_values, int_values, double_values) -
                       l(bool_values, int_values, double_values);
            };
        case statement_type::inequality:
            return [l, r](const std::vector<uint8_t> &bool_values,
                          const std::vector<int> &int_values,
                          const std::vector<double> &double_values) -> double {
                return -std::abs(l(bool_values, int_values, double_values) -
                                 r(bool_values, int_values, double_values));
            };
        default:
            throw std::runtime_error("Invalid statement type");
        }
    }
    void statement::c_code(int &, int &, int &, bool &,
                           std::stack<std::string> &, std::string &code) const {
        code += "mov aex 0\n";
    }

    const sym &statement::lhs() const { return child_nodes_.front(); }

    sym &statement::lhs() { return child_nodes_.front(); }

    const sym &statement::rhs() const { return child_nodes_.back(); }

    sym &statement::rhs() { return child_nodes_.back(); }

    statement::statement_type statement::statement_enum_type() const {
        return type_;
    }

    void statement::statement_enum_type(statement::statement_type t) {
        type_ = t;
    }

} // namespace sympp
