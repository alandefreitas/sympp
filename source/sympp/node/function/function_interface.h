// symbol.h

#ifndef SYMPP_SYMBOL
#define SYMPP_SYMBOL

// C++
#include <functional>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

// Internal
#include <sympp/core/internal_node_interface.h>
#include <sympp/core/sym.h>

namespace sympp {

    /// \class function node
    /// Abstract class to represent named functions and their parameters
    /// This is an abstract class because cannot know how to evaluate
    /// this function.
    class function_interface
        : public internal_node_interface<function_interface> {
      public:
        function_interface();
        function_interface(const function_interface &);
        function_interface(function_interface &&) noexcept;
        explicit function_interface(std::string_view fn_name);
        function_interface(std::string_view fn_name, int fn_parameters);
        function_interface(std::string_view fn_name, int fn_parameters,
                           bool commutative);
        function_interface(std::string_view fn_name,
                           const std::vector<sym> &fn_parameters);
        function_interface(std::string_view fn_name,
                           const std::vector<sym> &fn_parameters,
                           bool commutative);
        ~function_interface() override;

      public /* implement node interface */:
        [[nodiscard]] sym coeff(const node_interface &term) const override;
        [[nodiscard]] std::optional<sym> subs(const sym &x,
                                              const sym &y) override;

      public /* override internal node interface */:
        void stream(std::ostream &os, bool symbolic_format) const override;
        [[nodiscard]] bool is_commutative() const override;
      public /* specific to function_node */:
        std::optional<sym> subs(const sym &x, const int &y);
        std::optional<sym> subs(const sym &x, const double &y);
        void is_commutative(bool);
        sym operator~() const;
        [[nodiscard]] const std::string &name() const;

      private:
        /// Number of unnamed functions we created
        /// We can use this to create names for new unnamed functions
        static size_t n_unnamed_functions_;

        /// Generate name for a new function
        /// Try to define the name explicitly though.
        static std::string generate_function_name();

      private:
        /// Function name
        std::string name_;

        /// True if we can change the order of the function parameters
        /// without changing its meaning. This is rarely true.
        bool is_commutative_{false};
    };

} // namespace sympp

#endif
