#include "summation.h"
#include <sympp/functions/operators.h>
#include <sympp/functions/symbolic.h>
#include <sympp/node/operation/product.h>
#include <sympp/node/terminal/integer.h>
#include <sympp/node/terminal/number_interface.h>
#include <vector>

namespace sympp {

    summation::summation() = default;

    summation::summation(const summation &rhs) = default;

    summation::summation(const internal_node_interface<summation> &rhs) {
        auto srhs = dynamic_cast<const summation *>(&rhs);
        if (srhs) {
            *this = *srhs;
        } else {
            throw sym_error(sym_error::AbstractClass);
        }
    };

    summation::summation(summation &&v) noexcept
        : internal_node_interface<summation>(v){};

    summation::summation(const sym &a, const sym &b)
        : summation(std::vector<sym>({a, b})) {}

    summation::summation(const std::vector<sym> &child_nodes) {
        for (const auto &s : child_nodes) {
            if (s.is_summation()) {
                auto p = s.root_node_as<summation>();
                child_nodes_.insert(child_nodes_.end(), p->begin(), p->end());
            } else {
                child_nodes_.emplace_back(s);
            }
        }
    }

    summation::~summation() = default;

    summation &summation::operator=(const summation &rhs) {
        if (this != &rhs) {
            child_nodes_ = rhs.child_nodes_;
        }
        return *this;
    }

    sym summation::coeff(const node_interface &s) const {
        summation r;
        for (const auto &child_node : child_nodes_) {
            r.child_nodes_.emplace_back(child_node.coeff(s));
        }
        return sym(r);
    }

    double summation::evaluate(const std::vector<uint8_t> &bool_values,
                               const std::vector<int> &int_values,
                               const std::vector<double> &double_values) const {
        double value = 0.;
        for (const auto &summand : this->child_nodes_) {
            value += summand.evaluate(bool_values, int_values, double_values);
        }
        return value;
    }

    sym
    summation::evaluate_sym(const std::vector<uint8_t> &bool_values,
                            const std::vector<int> &int_values,
                            const std::vector<double> &double_values) const {
        sym value(0);
        for (const auto &summand : this->child_nodes_) {
            value +=
                summand.evaluate_sym(bool_values, int_values, double_values);
        }
        return value;
    }

    node_lambda summation::lambdify() const {
        std::vector<node_lambda> term_lambdas(this->child_nodes_.size());
        for (size_t i = 0; i < child_nodes_.size(); ++i) {
            term_lambdas[i] = child_nodes_[i].lambdify();
        }
        return
            [term_lambdas](const std::vector<uint8_t> &bool_values,
                           const std::vector<int> &int_values,
                           const std::vector<double> &double_values) -> double {
                double sum = 0.;
                for (const auto &t : term_lambdas) {
                    sum += t(bool_values, int_values, double_values);
                }
                return sum;
            };
    }

    void summation::c_code(int &function_level, int &sum_level, int &prod_level,
                           bool &leaf, std::stack<std::string> &code_aux,
                           std::string &code) const {
        sum_level++;
        code += " sum[" + std::to_string(sum_level) + "] = 0; \n";
        for (const auto &summand : this->child_nodes_) {
            if (summand.is_product()) {
                summand.root_node()->c_code(function_level, sum_level,
                                            prod_level, leaf, code_aux, code);
                if (prod_level == -1) {
                    code += " sum[" + std::to_string(sum_level) +
                            "] +=  prod[0]; \n";
                } else {
                    code += " sum[" + std::to_string(sum_level) +
                            "] +=  prod[" + std::to_string(prod_level + 1) +
                            "]; \n";
                }
            } else if (summand.is_variable() || summand.is_number()) {
                code += " sum[" + std::to_string(sum_level) + "] += ";
                summand.root_node()->c_code(function_level, sum_level,
                                            prod_level, leaf, code_aux, code);
                code += ";  \n";
            } else {
                code_aux.push(" sum[" + std::to_string(sum_level) + "] += ");
                summand.root_node()->c_code(function_level, sum_level,
                                            prod_level, leaf, code_aux, code);
            }
        }
        sum_level--;
    }

    std::optional<sym> summation::collect() {
        absorb_sum_of_sums();

        // Group common terms
        for (auto i = child_nodes_.begin(); i != child_nodes_.end(); ++i) {
            auto &child_node = *i;

            // Numbers will be grouped later
            if (child_node.type() == typeid(number_interface)) {
                continue;
            }

            // Create number with the leading coefficient of products
            sym leading_coefficient(1);

            // The leading coefficient of products must be ignored in
            // grouping comparisons
            if (child_node.type() == typeid(product)) {
                auto p = child_node.root_node_as<product>();
                const bool first_term_is_number =
                    !p->child_nodes_.empty() &&
                    p->child_nodes_.front().is_number();
                if (first_term_is_number) {
                    leading_coefficient = sym(p->child_nodes_.front());
                    p->child_nodes_.erase(p->child_nodes_.begin());
                    // Don't need to forward the ratio
                    // The child has already been simplified
                    // during collection, we just need to make
                    // sure the most obvious simplifications
                    // happen here.
                    p->simplify();
                }
            }

            // Iterate terms that come after child_node in this summation
            auto j = i;
            ++j;
            for (; j != child_nodes_.end();) {
                auto &child_node2 = *j;

                // Numbers will be grouped later
                if (child_node2.is_number()) {
                    ++j;
                    continue;
                }

                // Get the leading coefficient of this element too
                // The leading coefficient of products must be ignored
                // in grouping comparisons
                sym leading_coefficient2(integer(1));
                if (child_node2.type() == typeid(product)) {
                    auto p = child_node2.root_node_as<product>();
                    bool first_term_is_number =
                        !p->child_nodes_.empty() &&
                        p->child_nodes_.front().is_number();
                    if (first_term_is_number) {
                        leading_coefficient2 = sym(p->child_nodes_.front());
                        p->child_nodes_.erase(p->child_nodes_.begin());
                    }
                }

                // Compare the pair of child nodes
                // If the nodes (now without their number coefficients) are the
                // same
                if (child_node.root_node()->compare(*child_node2.root_node()) ==
                    0) {
                    // We create a new coefficient from both terms
                    leading_coefficient =
                        leading_coefficient + leading_coefficient2;
                    // and remove one of the terms (the second one)
                    j = child_nodes_.erase(j);
                } else {
                    ++j;
                }
            }

            // If the leading coefficient of the product is zero
            if (leading_coefficient.is_zero()) {
                // just erase the node
                i = child_nodes_.erase(i);
            } else {
                // if the leading coefficient is not zero
                // recreate the product with the new coefficient
                *i = (leading_coefficient * child_node);
                // simplify
                i->simplify();
                // and move forward
                ++i;
            }
        }

        return std::nullopt;
    }

    void summation::stream(std::ostream &os, bool symbolic_format) const {
        if (child_nodes_.empty()) {
            os << 0;
        } else {
            // if there are children
            for (auto i = child_nodes_.begin(); i != child_nodes_.end(); ++i) {
                if (i != child_nodes_.begin()) {
                    auto number_node = i->root_node_as<number_interface>();
                    bool is_positive_if_num =
                        !number_node || !number_node->is_negative();
                    auto prod_node = i->root_node_as<product>();
                    bool is_positive_if_prod =
                        !prod_node || !prod_node->prints_negative();
                    if (is_positive_if_num && is_positive_if_prod) {
                        os << "+";
                    }
                }
                i->stream(os, symbolic_format);
            }
        }
    }

    std::optional<sym> summation::simplify(double ratio,
                                           complexity_lambda measure_function) {
        /*
         * The ration and measure function are currently unused
         * because we don't use expand to try to simplify the
         * summation so `simplify` can only reduce the expression.
         */

        // Collect common powers (always reduces expression)
        collect();

        // Treat 1-element sum: (a) -> a (always reduces expression)
        if (child_nodes_.size() == 1) {
            child_nodes_.front().simplify(ratio, measure_function);
            return child_nodes_.front();
        }

        // Simplify children (forward the ratio)
        internal_node_interface<summation>::simplify(ratio, measure_function);

        // Collect common powers (always reduces expression)
        collect();

        // Treat 1-element sum: (a) -> a (always reduces expression)
        if (child_nodes_.size() == 1) {
            child_nodes_.front().simplify(ratio, measure_function);
            return child_nodes_.front();
        }

        // Move numbers to the back (always reduces expression)
        sym numbers(integer(0));
        for (auto j = child_nodes_.begin(); j != child_nodes_.end();) {
            if (j->is_number()) {
                numbers = numbers + sym(*j);
                j = child_nodes_.erase(j);
            } else {
                ++j;
            }
        }

        bool not_zero = !numbers.is_number() ||
                        !numbers.root_node_as<number_interface>()->is_zero();
        if (not_zero) {
            numbers.simplify(ratio, measure_function);
            child_nodes_.push_back(numbers);
        }

        if (child_nodes_.empty()) {
            return sym(0);
        }

        if (child_nodes_.size() == 1) {
            return child_nodes_.front();
        }

        return std::nullopt;
    }

    std::optional<sym> summation::subs(const sym &x, const sym &y) {
        if (x.is<summation>()) {
            auto x_ptr = x.root_node_as<summation>();
            if (children_match(*x_ptr)) {
                // reset the simplified and expanded flags
                // since substitution may have changed this
                if (y.is<summation>()) {
                    *this = *y.root_node_as<summation>();
                } else {
                    return y;
                }
            }
        }

        // sum does not contain expression for substitution
        // try to substitute in each summand
        for (auto &child_node : child_nodes_) {
            std::optional<sym> r = child_node.root_node()->subs(x, y);
            if (r) {
                child_node = *r;
            }
        }

        if (child_nodes_.size() == 1) {
            return *child_nodes_.begin();
        }
        return std::nullopt;
    }

    int summation::compare(const node_interface &s) const {
        // compare type
        if (type() != s.type()) {
            return (type().before(s.type())) ? -1 : +1;
        }

        // compare number of children
        const auto &rhs = dynamic_cast<const summation &>(s);
        if (child_nodes_.size() != rhs.child_nodes_.size()) {
            return child_nodes_.size() < rhs.child_nodes_.size() ? -1 : +1;
        }

        // compare the children
        if (children_match(rhs)) {
            return 0;
        } else {
            // if they don't match
            // the first inequality tells us who comes before
            auto i = this->child_nodes_.begin();
            auto j = rhs.child_nodes_.begin();
            int c;
            do {
                c = i->compare(*j);
                if (c != 0) {
                    return c;
                }
                ++i;
                ++j;
            } while (i == child_nodes_.end());
        }

        return 0;
    }

    void summation::put_indexes(
        std::unordered_map<int, sym> &bool_symbols_vars,
        std::unordered_map<int, sym> &int_symbols_vars,
        std::unordered_map<int, sym> &real_symbols_vars,
        std::unordered_map<std::string, int> &bool_symbols_names,
        std::unordered_map<std::string, int> &int_symbols_names,
        std::unordered_map<std::string, int> &real_symbols_names) {
        for (auto &summand : this->child_nodes_) {
            summand.root_node()->put_indexes(
                bool_symbols_vars, int_symbols_vars, real_symbols_vars,
                bool_symbols_names, int_symbols_names, real_symbols_names);
        }
    }

    bool summation::is_commutative() const {
        // summation
        return true;
    }

    void summation::absorb_sum_of_sums() {
        // find all internal sums
        auto is_not_sum = [](const auto &child) {
            return child.type() == typeid(summation);
        };
        auto p = std::stable_partition(child_nodes_.begin(), child_nodes_.end(),
                                       is_not_sum);
        std::vector<sym> internal_sums(
            std::make_move_iterator(p),
            std::make_move_iterator(child_nodes_.end()));
        child_nodes_.erase(p, child_nodes_.end());

        // move their terms to the main sum
        for (auto &internal_sum : internal_sums) {
            child_nodes_.insert(child_nodes_.end(),
                                std::make_move_iterator(internal_sum.begin()),
                                std::make_move_iterator(internal_sum.end()));
        }
    }

    bool summation::children_match(const summation &rhs) const {
        auto hint_it = child_nodes_.begin();
        for (auto &rhs_child : rhs.child_nodes_) {
            // search the term in this
            auto j = std::find(hint_it, child_nodes_.end(), rhs_child);
            if (j == child_nodes_.end()) {
                j = std::find(child_nodes_.begin(), child_nodes_.end(),
                              rhs_child);
            }
            // if not found
            if (j == child_nodes_.end()) {
                // give up
                break;
            } else {
                ++hint_it;
            }
        }
        return hint_it == child_nodes_.end();
    }

    std::optional<sym> summation::expand() {
        for (auto &child_node : child_nodes_) {
            std::optional<sym> r = child_node.root_node()->expand();
            if (r) {
                child_node = *r;
            }
        }
        return std::nullopt;
    }

    node_interface *summation::clone() const {
        return dynamic_cast<node_interface *>(new summation(*this));
    }

} // namespace sympp