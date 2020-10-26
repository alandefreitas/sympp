#include "product.h"
#include <algorithm>
#include <sympp/core/sym.h>
#include <sympp/core/sym_error.h>
#include <sympp/functions/operators.h>
#include <sympp/functions/symbolic.h>
#include <sympp/node/function/cos.h>
#include <sympp/node/function/function_interface.h>
#include <sympp/node/function/log.h>
#include <sympp/node/function/pow.h>
#include <sympp/node/function/sin.h>
#include <sympp/node/operation/summation.h>
#include <sympp/node/terminal/constant.h>
#include <sympp/node/terminal/integer.h>
#include <sympp/node/terminal/real.h>
#include <vector>

namespace sympp {

    product::product() = default;

    product::product(const product &) = default;

    product::product(const internal_node_interface<product> &s) {
        auto prhs = dynamic_cast<const product *>(&s);
        if (prhs) {
            *this = *prhs;
        } else {
            throw sym_error(sym_error::AbstractClass);
        }
    }

    product::product(product &&v) noexcept
        : internal_node_interface<product>(v) {}

    product::product(const sym &a, const sym &b)
        : product(std::vector<sym>({a, b})) {}

    product::product(const std::vector<sym> &child_nodes) {
        for (const auto &s : child_nodes) {
            if (s.is_product()) {
                auto p = s.root_node_as<product>();
                child_nodes_.insert(child_nodes_.end(), p->begin(), p->end());
            } else {
                child_nodes_.emplace_back(s);
            }
        }
    }

    product::product(std::vector<sym> &&child_nodes)
        : internal_node_interface<product>(std::move(child_nodes)) {}

    product::~product() = default;

    product &product::operator=(const product &rhs) {
        if (child_nodes_ != rhs.child_nodes_) {
            child_nodes_ = rhs.child_nodes_;
        }
        return *this;
    }

    sym product::coeff(const node_interface &s) const {
        // This returns which coefficient/child_nodes_ are
        // multiplying by the variable s. The product is
        // considered to be is a single term in a sum.

        // If we find s in the product, we have the rest
        // of the product as its coefficient.
        // If we find s twice, however, we return 0,
        // because that would be a coefficient of s^2.

        // Look for s
        auto is_s = [&s](const sym &c) {
            if (c.root_node()->type() != s.type()) {
                return false;
            }
            return c.root_node()->compare(s) == 0;
        };
        auto s_it =
            std::find_if(child_nodes_.begin(), child_nodes_.end(), is_s);

        // If not found even once there's no coefficient for
        // s in this product
        bool found_s = s_it != child_nodes_.end();
        if (!found_s) {
            return sym(0);
        }

        // Look for s again
        ++s_it;
        auto s_it2 = std::find_if(s_it, child_nodes_.end(), is_s);

        // If found again, this is a coefficient for s^2, s^3, ...
        // but not for s
        found_s = s_it2 != child_nodes_.end();
        if (found_s) {
            return sym(0);
        }

        // Make a copy and remove s from it
        product copy_without_s(*this);
        size_t s_pos = s_it - child_nodes_.begin();
        copy_without_s.child_nodes_.erase(copy_without_s.begin() + s_pos);
        return sym(std::move(copy_without_s));
    }

    double product::evaluate(const std::vector<uint8_t> &bool_values,
                             const std::vector<int> &int_values,
                             const std::vector<double> &double_values) const {
        double value = 1.0;
        for (const auto &factor : this->child_nodes_) {
            value *= factor.root_node()->evaluate(bool_values, int_values,
                                                  double_values);
        }
        return value;
    }

    sym product::evaluate_sym(const std::vector<uint8_t> &bool_values,
                              const std::vector<int> &int_values,
                              const std::vector<double> &double_values) const {
        sym value(integer(1));
        for (const auto &factor : this->child_nodes_) {
            value *= factor.root_node()->evaluate_sym(bool_values, int_values,
                                                      double_values);
        }
        return value;
    }

    node_lambda product::lambdify() const {
        // compile terms
        std::vector<node_lambda> term_lambdas(this->child_nodes_.size());
        for (size_t i = 0; i < child_nodes_.size(); ++i) {
            term_lambdas[i] = child_nodes_[0].root_node()->lambdify();
        }

        // create lambda that calculates all terms
        return
            [term_lambdas](const std::vector<uint8_t> &bool_values,
                           const std::vector<int> &int_values,
                           const std::vector<double> &double_values) -> double {
                double result = 1.;
                for (const auto &t : term_lambdas) {
                    result *= t(bool_values, int_values, double_values);
                }

                return result;
            };
    }

    void product::c_code(int &function_level, int &sum_level, int &prod_level,
                         bool &leaf, std::stack<std::string> &code_aux,
                         std::string &code) const {
        prod_level++;
        code += " prod[" + std::to_string(prod_level) + "] = 1; \n";
        for (auto &&factor : this->child_nodes_) {
            if (factor.is_summation()) {
                factor.root_node()->c_code(function_level, sum_level,
                                           prod_level, leaf, code_aux, code);
                if (sum_level == -1) {
                    code += " prod[" + std::to_string(prod_level) +
                            "] *=  sum[0]; \n";
                } else {
                    code += " prod[" + std::to_string(prod_level) +
                            "] *=  sum[" + std::to_string(sum_level + 1) +
                            "]; \n";
                }
            } else if (factor.is_variable() || factor.is_number()) {
                code += " prod[" + std::to_string(prod_level) + "] *= ";
                factor.root_node()->c_code(function_level, sum_level,
                                           prod_level, leaf, code_aux, code);
                code += ";  \n";
            } else {
                code_aux.push(" prod[" + std::to_string(prod_level) + "] *= ");
                factor.root_node()->c_code(function_level, sum_level,
                                           prod_level, leaf, code_aux, code);
            }
        }
        prod_level--;
    }

    void product::expand_powers() {
        // for each term
        std::vector<sym> expanded_powers;
        auto it = child_nodes_.begin();
        while (it != child_nodes_.end()) {
            // if it's power node
            if (it->type() == typeid(pow)) {
                // get pointer
                auto p = it->root_node_as<pow>();
                // if exponent is integer
                if (p->child_nodes_.back().is<integer>()) {
                    // get exponent as int
                    auto e = p->child_nodes_.back().root_node_as<integer>();
                    int n = static_cast<int>(*e);
                    // if exponent is more than 0
                    if (n > 0) {
                        // if base is product
                        if (p->child_nodes_.front().is_product()) {
                            auto b =
                                p->child_nodes_.front().root_node_as<product>();
                            for (int m = 0; m < n; ++m) {
                                expanded_powers.insert(expanded_powers.end(),
                                                       b->child_nodes_.begin(),
                                                       b->child_nodes_.end());
                            }
                        } else {
                            // if not product, push the base exponent times
                            for (int m = 0; m < n; ++m) {
                                expanded_powers.push_back(
                                    p->child_nodes_.front());
                            }
                        }
                        // remove the original power
                        it = child_nodes_.erase(it);
                    } else {
                        // if exponent is 0
                        // just do nothing
                        ++it;
                    }
                } else {
                    // if type is not int, do nothing
                    ++it;
                }
            } else {
                // if not power node, do nothing
                ++it;
            }
        }
    }

    std::optional<sym> product::expand() {
        // Expand summations
        // eg.: x(a+c+c)y -> a*xy + b*xy + c*xy
        for (auto i = child_nodes_.begin(); i != child_nodes_.end(); ++i) {
            // If summation
            i->expand();
            if (i->is_summation() && i->size() > 1) {
                // Replace the whole product with the summation p_1 + p_2 + ...
                //    where p_i = (terms up to summation) * sum_term * (terms
                //    after summation)
                //      or  p_i = summation_term * (terms without summation)
                std::vector<sym> other_terms(child_nodes_.begin(), i);
                other_terms.insert(other_terms.end(), std::next(i),
                                   child_nodes_.end());

                summation result;
                for (const auto &summand : *i) {
                    product result_summand(other_terms);
                    result_summand.child_nodes_.emplace_back(summand);
                    result.child_nodes_.emplace_back(result_summand);
                }

                auto r = result.expand();
                if (r) {
                    return *r;
                } else {
                    return sym(std::move(result));
                }
            }
        }
        return std::nullopt;
    }

    std::optional<sym> product::collect() {
        // Absorb product of product
        // eg.: a * (a * a) * a -> a * a * a * a

        auto i = child_nodes_.begin();
        while (i != child_nodes_.end()) {

            sym &s = *i;
            if (s.is_product()) {
                auto p = s.root_node_as<product>();
                child_nodes_.insert(child_nodes_.end(), p->child_nodes_.begin(),
                                    p->child_nodes_.end());
                i = child_nodes_.erase(i);
            } else {
                ++i;
            }
        }

        // Group common terms
        // eg.: x*x*x -> x^3
        for (i = child_nodes_.begin(); i != child_nodes_.end(); ++i) {

            // numbers will be grouped later
            if (i->is_number()) {
                continue;
            }

            // Total power of this node
            sym total_power(1);

            // The exponent in products must be ignored in grouping
            // comparisons. Just consider the base, keep the exponent
            // and keep going as usual
            sym *child_node = &(*i);
            if (child_node->is<pow>()) {
                auto p = child_node->root_node_as<pow>();
                total_power = p->child_nodes_.back();
                child_node = &(p->child_nodes_.front());
            }

            auto j = i;
            ++j;
            while (j != child_nodes_.end() && i->is_commutative()) {
                // Numbers will be grouped later
                if (j->is_number()) {
                    ++j;
                    continue;
                }

                sym *child_node2 = &(*j);
                sym total_power2(1);

                // The exponent in products must be ignored in grouping
                // comparisons
                if (child_node2->is<pow>()) {
                    auto p = child_node2->root_node_as<pow>();
                    total_power2 = p->child_nodes_.back();
                    child_node2 = &(p->child_nodes_.front());
                }

                // If the same, update the total power
                if (child_node->compare(*child_node2) == 0) {
                    total_power = total_power + total_power2;
                    j = child_nodes_.erase(j);
                } else {
                    ++j;
                }
            }

            if (total_power == 0) {
                *i = 1;
            } else if (total_power == 1) {
                *i = *child_node;
            } else {
                *i = sym(pow(*child_node, total_power));
                i->simplify();
            }
        }

        // Move numbers to the front (always reduces expression)
        sym numbers(real(1.0));
        for (auto j = child_nodes_.begin(); j != child_nodes_.end();) {
            if (j->is_number()) {
                numbers = numbers * sym(*j);
                if (numbers.is_number() &&
                    numbers.root_node_as<number_interface>()->is_zero()) {
                    return numbers;
                }
                j = child_nodes_.erase(j);
            } else {
                ++j;
            }
        }

        bool not_one = !numbers.is_number() ||
                       !numbers.root_node_as<number_interface>()->is_one();

        std::cout << " numbers " << numbers << std::endl << std::endl;

        if (not_one) {

            numbers.simplify();

            child_nodes_.insert(child_nodes_.begin(), numbers);
        }

        return std::nullopt;
    }

    void product::stream(std::ostream &os, bool symbolic_format) const {
        if (child_nodes_.empty()) {
            os << 1;
        } else if (child_nodes_.size() == 1) {
            child_nodes_.begin()->stream(os, symbolic_format);
        } else {
            for (auto i = child_nodes_.begin(); i != child_nodes_.end(); ++i) {
                if (i != child_nodes_.begin()) {
                    os << "*";
                }
                if (*i == -1) {
                    os << "-";
                    ++i;
                }
                if (i->type() != typeid(product) && i->type() != typeid(pow) &&
                    i->type() != typeid(sin) && i->type() != typeid(cos) &&
                    i->type() != typeid(log) && !i->is_variable() &&
                    !i->is_function() && !i->is_number() && !i->is_constant()) {
                    os << "(";
                    i->stream(os, symbolic_format);
                    os << ")";
                } else {
                    i->stream(os, symbolic_format);
                }
            }
        }
    }

    std::optional<sym> product::simplify(double ratio) {
        return node_interface::simplify(ratio);
    }

    std::optional<sym> product::simplify() {
        return node_interface::simplify();
    }

    std::optional<sym> product::simplify(double ratio,
                                         complexity_lambda measure_function) {

        // Collect common powers (always reduces expression)
        // eg.: x*x*x*x -> x^4
        collect();

        // Treat 1-element sum: (a) -> a (always reduces expression)
        if (child_nodes_.size() == 1) {
            child_nodes_.front().simplify(ratio, measure_function);
            return child_nodes_.front();
        }

        // Simplify children (forward the ratio)
        internal_node_interface<product>::simplify(ratio, measure_function);

        // Collect common powers (always reduces expression)
        collect();

        // Treat 1-element sum: (a) -> a (always reduces expression)
        if (child_nodes_.size() == 1) {
            child_nodes_.front().simplify(ratio, measure_function);
            return child_nodes_.front();
        }

        if (child_nodes_.empty()) {
            return sym(1);
        }

        if (child_nodes_.size() == 1) {
            return child_nodes_.front();
        }

        return std::nullopt;
    }

    std::optional<sym> product::subs(const sym &x, const sym &y) {
        // Try to replace the root directly
        if (this->compare(*x.root_node()) == 0) {
            if (y.is<product>()) {
                child_nodes_ = y.root_node_as<product>()->child_nodes_;
                return std::nullopt;
            } else {
                return y;
            }
        }

        // Try to replace the children directly
        for (auto &child_node : child_nodes_) {
            if (child_node.compare(x) == 0) {
                child_node = y;
            }
        }

        bool x_is_also_product = x.type() == type();
        if (x_is_also_product) {
            // Expand terms and try to replace again
            product x_copy = *x.root_node_as<product>();
            product this_copy = *this;
            x_copy.expand_powers();
            this_copy.expand_powers();

            // Look for all matches between subsets of
            // this_copy and the complete x_copy

            // 1) Identify and count unique terms of x_copy
            std::vector<std::pair<sym, size_t>> x_unique;
            for (auto &t : x_copy) {
                auto it = std::find_if(
                    x_unique.begin(), x_unique.end(),
                    [&t](const std::pair<sym, size_t> &unique_term) {
                        return unique_term.first.compare(t) == 0;
                    });
                if (it != x_unique.end()) {
                    x_unique.emplace_back(t, 0);
                } else {
                    ++it->second;
                }
            }

            // 2) Count elements of this_copy that match unique elements of x
            //    we keep track of where we found them
            std::vector<std::vector<std::vector<sym>::iterator>>
                found_positions(x_unique.size());
            for (auto i = this_copy.child_nodes_.begin();
                 i != this_copy.child_nodes_.end(); ++i) {
                auto &child_node = *i;
                auto it = std::find_if(
                    x_unique.begin(), x_unique.end(),
                    [&child_node](const std::pair<sym, size_t> &unique_term) {
                        return unique_term.first.compare(child_node) == 0;
                    });
                bool found_x_unique_term_in_this = it != x_unique.end();
                if (found_x_unique_term_in_this) {
                    found_positions[it - x_unique.begin()].emplace_back(i);
                }
            }

            // 3) Identify which elements we should remove from this
            std::vector<std::vector<sym>::iterator> its_to_erase;
            auto calc_if_enough = [&found_positions, &x_unique]() {
                for (size_t i = 0; i < found_positions.size(); ++i) {
                    if (found_positions[i].size() < x_unique[i].second) {
                        return false;
                    }
                }
                return true;
            };
            bool enough_for_substitution = calc_if_enough();
            size_t substitutions = 0;
            while (enough_for_substitution) {
                for (size_t i = 0; i < found_positions.size(); ++i) {
                    for (size_t j = 0; j < x_unique[i].second; ++j) {
                        its_to_erase.emplace_back(found_positions[i].back());
                        its_to_erase.pop_back();
                    }
                }
                enough_for_substitution = calc_if_enough();
                ++substitutions;
            }

            // 4) Remove the elements from this_copy
            // (sort in decreasing order not to invalidate the iterators)
            std::sort(
                its_to_erase.begin(), its_to_erase.end(),
                [](const auto &ita, const auto &itb) { return itb > ita; });
            for (auto &it_to_erase : its_to_erase) {
                this_copy.child_nodes_.erase(it_to_erase);
            }

            // 5) Include "substitutions" copies of y in this_copy
            for (size_t i = 0; i < substitutions; ++i) {
                this_copy.child_nodes_.emplace_back(y);
            }

            // 6) Move this_copy back to this
            child_nodes_ = std::move(this_copy.child_nodes_);
        }

        // Call subs on all children that are left in the product
        // as usual and return
        return internal_node_interface<product>::subs(x, y);
    }

    bool product::children_match(const product &rhs) const {
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

    int product::compare(const node_interface &s) const {
        if (type() != s.type()) {
            return type().before(s.type()) ? -1 : 1;
        }
        const auto &rhs = dynamic_cast<const product &>(s);

        // compare the children
        if (children_match(rhs)) {
            return 0;
        } else {
            // if they don't match
            // the first inequality tells us who should come before
            auto i = this->child_nodes_.begin();
            auto j = rhs.child_nodes_.begin();
            int c = 0;
            while (i != child_nodes_.end()) {
                c = i->compare(*j);
                if (c != 0) {
                    return c;
                } else {
                    ++i;
                    ++j;
                }
            }
        }
        return 0;
    }

    void product::put_indexes(
        std::unordered_map<int, sym> &bool_symbols_vars,
        std::unordered_map<int, sym> &int_symbols_vars,
        std::unordered_map<int, sym> &real_symbols_vars,
        std::unordered_map<std::string, int> &bool_symbols_names,
        std::unordered_map<std::string, int> &int_symbols_names,
        std::unordered_map<std::string, int> &real_symbols_names) {
        for (auto &factor : this->child_nodes_) {
            factor.root_node()->put_indexes(
                bool_symbols_vars, int_symbols_vars, real_symbols_vars,
                bool_symbols_names, int_symbols_names, real_symbols_names);
        }
    }

    bool product::is_commutative() const {
        // https://www.dpmms.cam.ac.uk/~wtg10/commutative.html
        // product is commutative *unless* we are working with
        // matrices here
        return true;
    }

    int product::prints_negative() const {
        if (child_nodes_.size() <= 1) {
            return false;
        }

        if (!child_nodes_.front().is_number()) {
            return false;
        }

        auto p = child_nodes_.front().root_node_as<number_interface>();
        return p->is_negative();
    }

} // namespace sympp