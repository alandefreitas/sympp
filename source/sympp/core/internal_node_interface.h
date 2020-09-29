//
// Created by Alan Freitas on 18/09/20.
//

#ifndef SYMPP_INTERNAL_NODE_INTERFACE_H
#define SYMPP_INTERNAL_NODE_INTERFACE_H

// C++
#include <algorithm>
#include <numeric>

// Internal
#include <sympp/core/node_interface.h>
#include <sympp/core/sym.h>

namespace sympp {

    /// \class Internal node interface
    /// This is a derivation from node_interface.
    /// with default implementations for internal nodes
    template <class DERIVED>
    class internal_node_interface : public node_interface {
      public:
        internal_node_interface() : child_nodes_(0){};
        explicit internal_node_interface(const std::vector<sym> &children)
            : child_nodes_(children) {}
        explicit internal_node_interface(std::vector<sym> &&children)
            : child_nodes_(std::move(children)) {}
        ~internal_node_interface() override = default;

      public /* virtual placeholders derived classes should override */:
        void stream(std::ostream &os, bool symbolic_format) const override {
            os << typeid(DERIVED).name();
            os << "(";
            for (size_t i = 0; i < child_nodes_.size(); ++i) {
                if (i != 0) {
                    os << ", ";
                }
                child_nodes_[i].stream(os, symbolic_format);
            }
            os << ")";
        };

        /// Simplify expression
        std::optional<sym> simplify(double ratio,
                                    complexity_lambda func) override {
            // By default, we try to simplify all children
            // but we don't know how to simplify this very symbol.
            // This is a placeholder when we don't know how to
            // simplify it. If all symbols do that, nothing gets
            // simplified ever.
            for (auto &child_node : child_nodes_) {
                std::optional<sym> r =
                    child_node.root_node()->simplify(ratio, func);
                if (r) {
                    child_node = sym(*r);
                }
            }
            return std::nullopt;
        }

        std::optional<sym> expand() override {
            for (auto &child_node : child_nodes_) {
                child_node.expand();
            }
            return std::nullopt;
        }

        /// Substitute instances of a variable in a expression
        std::optional<sym> subs(const sym &x, const sym &y) override {
            if (this->compare(*x.root_node()) == 0) {
                return y;
            }
            for (auto &child_node : child_nodes_) {
                std::optional<sym> r = child_node.root_node()->subs(x, y);
                if (r) {
                    child_node = *r;
                }
            }
            return std::nullopt;
        }

        [[nodiscard]] int compare(const node_interface &s) const override {
            // first criterion is type
            if (type() != s.type()) {
                return (type().before(s.type())) ? -1 : +1;
            }

            // second criterion is number of child nodes
            using self_t = decltype(*this);
            const auto &rhs = dynamic_cast<const self_t>(s);
            if (child_nodes_.size() != rhs.child_nodes_.size()) {
                return (child_nodes_.size() < rhs.child_nodes_.size()) ? -1
                                                                       : +1;
            }

            // third criterion is if any child compares != 0
            if (!is_commutative()) {
                for (size_t i = 0; i < child_nodes_.size(); ++i) {
                    int r = child_nodes_[i].compare(rhs.child_nodes_[i]);
                    if (r != 0) {
                        return r;
                    }
                }
            } else {
                std::vector<uint8_t> pos_where_found(child_nodes_.size(),
                                                     false);
                for (const auto &rhs_child : rhs.child_nodes_) {
                    auto this_begin = child_nodes_.begin();
                    while (this_begin != child_nodes_.end()) {
                        auto is_equal = [&](const auto &a) {
                            return a.compare(rhs_child) == 0;
                        };
                        auto i = std::find_if(this_begin, child_nodes_.end(),
                                              is_equal);
                        bool not_found = i == child_nodes_.end();
                        if (not_found) {
                            int inequality_value =
                                child_nodes_.begin()->compare(rhs_child);
                            return inequality_value != 0 ? inequality_value
                                                         : -1;
                        } else {
                            size_t found_pos = i - child_nodes_.end();
                            const bool first_time_finding_it =
                                !pos_where_found[found_pos];
                            if (first_time_finding_it) {
                                pos_where_found[found_pos] = true;
                                this_begin = child_nodes_.end();
                            } else {
                                this_begin = std::next(i);
                            }
                        }
                    }
                }
            }

            // if equal in all criteria, return 0 (they are equal)
            return 0;
        }

        /// Number of terms in an expression
        [[nodiscard]] size_t size() const override {
            // only one term in a terminal expression
            return child_nodes_.size();
        }

        /// Number of operations in the whole expression tree
        [[nodiscard]] size_t count_ops() const override {
            // by default, sum the operations in the children
            // if this is combined with terminal_node_interface,
            // we assume we have one operation per terminal node
            // this should be accurate enough for most applications
            return std::accumulate(child_nodes_.begin(), child_nodes_.end(),
                                   size_t{0}, [](size_t s, const sym &child) {
                                       return s + child.count_ops();
                                   });
        }

        /// Put indexes in the symbol
        void put_indexes(
            std::unordered_map<int, sym> &bool_symbols_vars,
            std::unordered_map<int, sym> &int_symbols_vars,
            std::unordered_map<int, sym> &real_symbols_vars,
            std::unordered_map<std::string, int> &bool_symbols_names,
            std::unordered_map<std::string, int> &int_symbols_names,
            std::unordered_map<std::string, int> &real_symbols_names) override {
            // By default, assume only the children might have indexes
            for (auto &child_node : child_nodes_) {
                child_node.root_node()->put_indexes(
                    bool_symbols_vars, int_symbols_vars, real_symbols_vars,
                    bool_symbols_names, int_symbols_names, real_symbols_names);
            }
        }

      public /* virtual placeholders derived classes probably don't need to
                override */
          :
        /// Get the type of the derived class
        /// If your class is abstract, it won't inherit this function
        [[nodiscard]] const std::type_info &type() const override {
            if (!std::is_abstract_v<DERIVED>) {
                return typeid(DERIVED);
            } else {
                throw std::logic_error(
                    std::string(typeid(DERIVED).name()) +
                    "::type() should not return for abstract types");
            }
        }

        /// Create a copy of this node and return a pointer as node_interface
        /// If your class is abstract, it won't inherit this function
        [[nodiscard]] node_interface *clone() const override {
            if constexpr (!std::is_abstract_v<DERIVED>) {
                return dynamic_cast<node_interface *>(new DERIVED(*this));
            } else {
                throw std::logic_error(
                    std::string(typeid(DERIVED).name()) +
                    "::clone() should not return for abstract types");
            }
        }

        [[nodiscard]] bool is_commutative() const override { return false; }

        /// Return true if this is a terminal node
        [[nodiscard]] bool is_terminal() const override { return false; }

        std::vector<sympp::sym>::iterator begin() override {
            return child_nodes_.begin();
        }
        std::vector<sympp::sym>::iterator end() override {
            return child_nodes_.end();
        }

        [[nodiscard]] std::vector<sympp::sym>::const_iterator
        begin() const override {
            return child_nodes_.begin();
        }

        [[nodiscard]] std::vector<sympp::sym>::const_iterator
        end() const override {
            return child_nodes_.end();
        }

      protected:
        /// Terms of this operation
        std::vector<sym> child_nodes_;
    };
} // namespace sympp

#endif // SYMPP_INTERNAL_NODE_INTERFACE_H
