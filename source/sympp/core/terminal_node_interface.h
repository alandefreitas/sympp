//
// Created by Alan Freitas on 18/09/20.
//

#ifndef SYMPP_TERMINAL_NODE_INTERFACE_H
#define SYMPP_TERMINAL_NODE_INTERFACE_H

#include <sympp/core/node_interface.h>
#include <sympp/core/sym.h>
#include <sympp/core/sym_error.h>

namespace sympp {

    /// \class Terminal node interface
    /// This is a derivation from node_interface.
    /// with default implementations for terminal nodes
    /// Virtual functions derived classes still need to implement:
    template <class DERIVED>
    class terminal_node_interface : public node_interface {
      public:
        ~terminal_node_interface() override = default;
      public /* virtual placeholders derived classes should probably override */
          :
        /// stream the name of the derived class as a placeholder for `stream`
        void stream(std::ostream &os, [[maybe_unused]] bool) const override {
            os << typeid(DERIVED).name();
        };

        /// Number of terms in an expression
        [[nodiscard]] size_t size() const override {
            // only one term in a terminal expression
            return 1;
        }

        /// Number of operations in the whole expression tree
        [[nodiscard]] size_t count_ops() const override {
            // the size is the only thing here
            return 1;
        }

      public /* virtual placeholders derived classes probably don't need to
                override */
          :
        /// Get the type of the derived class
        [[nodiscard]] const std::type_info &type() const override {
            return typeid(DERIVED);
        }

        /// Substitute instances of a variable in a expression
        /// That's all there is to substitution of terminal variables
        std::optional<sym> subs(const sym &x, const sym &y) override {
            if (this->compare(*x.root_node()) == 0) {
                return y;
            }
            return std::nullopt;
        }

        /// Commutative symbol
        [[nodiscard]] bool is_commutative() const override { return true; }

        /// Simplify expression
        std::optional<sym>
        simplify([[maybe_unused]] double ratio,
                 [[maybe_unused]] complexity_lambda func) override {
            // there's nothing to simplify in an terminal node
            return std::nullopt;
        }

        /// Create a copy of this node and return a pointer as node_interface
        [[nodiscard]] node_interface *clone() const override {
            if constexpr (!std::is_abstract_v<DERIVED>) {
                return dynamic_cast<node_interface *>(new DERIVED(*this));
            } else {
                throw sym_error(sym_error::AbstractClass);
            }
        }

        /// Return true if this is a terminal node
        [[nodiscard]] bool is_terminal() const override { return true; }
    };
} // namespace sympp

#endif // SYMPP_TERMINAL_NODE_INTERFACE_H
