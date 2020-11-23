// sym.h

#ifndef SYMPP_SYMBOLIC
#define SYMPP_SYMBOLIC

// C++
#include <cstddef>
#include <functional>
#include <iostream>
#include <memory>
#include <type_traits>
#include <typeinfo>
#include <unordered_map>
#include <vector>

// Internal
// You can't include node_interface here
// because that would create dependency
// cycles. Forward-declare node_interface.

namespace sympp {
    /// Forward declare partial definitions
    /// sym.h cannot include node headers because
    /// node headers need to include sym.h
    class node_interface;

    template <class T> class number;

    class number_interface;

    class function_interface;

    class boolean;

    class integer;

    class real;

    class rational;

    class variable;

    class constant;

    class summation;

    class product;

    class abs;

    class cos;

    class cosh;

    class log;

    class pow;

    class sin;

    class sinh;

    class statement;

    /// Type of a tiny c compiler function
    typedef double (*tcc_function)(bool *, int *, double *);

    /// Return type of the lambdify function
    using node_lambda = std::function<double(const std::vector<uint8_t> &,
                                             const std::vector<int> &,
                                             const std::vector<double> &)>;

    /// Function measuring the complexity of a symbol
    using complexity_lambda = std::function<double(const node_interface &)>;

    /// Numbers can be int, bool, or real
    enum numeric_type { var_integer, var_boolean, var_real };

    /// Function type
    enum function_type { linear, nonlinear, unknown };

    /// \class Symbolic object
    /// This is the main class in our library.
    /// That's the only object the user has to manage.
    /// It represents a symbol with a tree with nodes.
    /// Nodes are defined by the node_interface.h
    /// A sym contains:
    /// 1) the same functions as a node_interface, in which
    ///    case the calls are forwarded to the parent node
    /// 2) some extra functions to manipulate and lambdify
    ///    the symbols more conveniently.
    class sym {
      public /* constructors */:
        /// Construct an empty sym
        sym();

        /// Construct a sym from another sym
        sym(const sym &);

        /// Construct a sym from another sym
        sym(sym &&) noexcept;

        /// Construct a sym from any object that inherits from node_interface
        explicit sym(const node_interface &);

        /// Construct a sym from any object that inherits from node_interface
        explicit sym(boolean &&);

        explicit sym(integer &&);

        explicit sym(real &&);

        explicit sym(rational &&);

        explicit sym(variable &&);

        explicit sym(constant &&);

        explicit sym(summation &&);

        explicit sym(product &&);

        explicit sym(abs &&);

        explicit sym(cos &&);

        explicit sym(cosh &&);

        explicit sym(log &&);

        explicit sym(pow &&);

        explicit sym(sin &&);

        explicit sym(sinh &&);

        explicit sym(statement &&);

        /// Construct a sym of an specific type
        template <class NODE_TYPE, class... Args>
        explicit sym(Args &&... args)
            : root_node_(dynamic_cast<node_interface *>(
                  new NODE_TYPE(std::forward<Args>(args)...))) {}

        /// Construct a number from a bool
        explicit sym(bool);

        /// Construct a number from an int
        explicit sym(int);

        /// Construct a number from a double
        explicit sym(double);

        /// Construct a constant from a symbol name
        explicit sym(std::string_view, const number_interface &);

        /// Construct a constant from a symbol name
        explicit sym(std::string_view, bool);

        /// Construct a constant from a symbol name
        explicit sym(std::string_view, int);

        /// Construct a constant from a symbol name
        explicit sym(std::string_view, double);

        /// Construct a constant from a symbol name
        explicit sym(std::string_view, const sym &);

        /// Construct variable from a string
        explicit sym(std::string_view);

        /// Construct variable from a std::string
        explicit sym(std::string_view, numeric_type number_type);

        /// Forward any constructor with char* to a string_view
        template <class... Args>
        explicit sym(const char *str, Args &&... args)
            : sym(std::string_view(str), std::forward<Args>(args)...) {}

        explicit sym(numeric_type number_type, std::string_view);

        /// Destructor
        ~sym();

      public /* simplification */:
        /// Simplify expression regardless of ratio
        /// Perform only most obvious/trivial simplifications
        sym &simplify();

        /// Don't allow the expresion to grow more than ration
        sym &simplify(double ratio);

        /// Don't allow expression to grow more than ratio
        /// and use function as a complexity measure of the
        /// expression for ratio
        sym &simplify(double ratio, complexity_lambda func);

        /// Put polynomial into a canonical form of a sum of monomials
        sym &expand();

        /// Takes a polynomial and factors it into irreducible factors
        sym &factor();

        /// Collects common powers of a term in an expression
        sym &collect();

        /// Take any rational function and put it into the standard canonical
        /// form
        sym &cancel();

        /// Performs a partial fraction decomposition on a rational function
        sym &appart();

        /// Simplify expressions using trigonometric identities
        sym &trigsimp();

        /// Expand trigonometric functions, that is, apply the sum or double
        /// angle identities
        sym &expand_trig();

        /// Applies identities x^ax^b=x^{a+b} and x^ay^a=(xy)^a from left to
        /// right Identity x^ay^a=(xy)^a is true if at least x and y are
        /// non-negative and a is real.
        sym &powsimp();

        /// Applies identity x^ax^b=x^{a+b} from right to left
        sym &expand_power_exp();

        /// Applies identity x^ay^a=(xy)^a from right to left
        /// Identity x^ay^a=(xy)^a is true if at least x and y are non-negative
        /// and a is real.
        sym &expand_power_base();

        /// Applies identity x^a^b=x^{ab} from left to right
        /// Identity x^a^b=x^{ab} is true if b is an integer
        sym &powdenest();

        /// Apply identities log(xy)=log(x)+log(y) and log(x^n)=nlog(x) from
        /// left to right Only if x and y are positive and n is real.
        sym &expand_log();

        /// Apply identities log(xy)=log(x)+log(y) and log(x^n)=nlog(x) from
        /// right to left Only if x and y are positive and n is real.
        sym &logcombine();

      public /* manipulate symbol */:
        /// Stream the node to ostream
        void stream(std::ostream &, bool print_format_symbolic) const;

        /// Substitute instances of a variable in a expression
        /// The symbol might need to change its type
        sym &subs(const sym &, const sym &);

        sym &subs(const sym &, bool);

        sym &subs(const sym &, int);

        sym &subs(const sym &, double);

        sym &subs(const sym &);

        sym &subs(const statement &);

        sym &subs(const std::vector<statement> &);

        /// Compare two symbols for <, ==, >
        /// This is the spaceship operator <=>
        [[nodiscard]] int compare(const sym &) const;

        /// Compare two symbols for <, ==, >
        /// This is the spaceship operator <=>
        [[nodiscard]] int compare(const node_interface &) const;

        /// Get coefficients
        [[nodiscard]] sym coeff(const sym &) const;

        [[nodiscard]] sym coeff(const node_interface &) const;

        [[nodiscard]] sym coeff(const sym &, int) const;

        [[nodiscard]] sym coeff(int) const;

        [[nodiscard]] sym coeff(double) const;

        /// Commutative value
        [[nodiscard]] bool is_commutative() const;

        /// Number of terms in the symbol
        [[nodiscard]] size_t size() const;

        /// Number of operations in the while symbolic tree
        [[nodiscard]] size_t count_ops() const;

        /// Identify function category
        function_type calculate_function_type();

        /// Iterate the terms of a symbol
        std::vector<sym>::iterator begin();

        /// Iterate the terms of a symbol
        std::vector<sym>::iterator end();

        /// Iterate the terms of a symbol
        [[nodiscard]] std::vector<sym>::const_iterator begin() const;

        /// Iterate the terms of a symbol
        [[nodiscard]] std::vector<sym>::const_iterator end() const;

      public /* compile and evaluate */:
        /// Put indexes in the symbol
        /// Symbols need indexes so that we can evaluate them
        void put_indexes();

        /// Evaluate expression to a number
        [[nodiscard]] double
        evaluate(const std::vector<uint8_t> &bool_values,
                 const std::vector<int> &int_values,
                 const std::vector<double> &double_values) const;

        /// Evaluate expression to a symbol
        /// The resulting symbol is expected to be a
        /// single number
        [[nodiscard]] sym
        evaluate_sym(const std::vector<uint8_t> &bool_values,
                     const std::vector<int> &int_values,
                     const std::vector<double> &double_values) const;

        /// Compile expression to a function pointer
        [[nodiscard]] node_lambda lambdify() const;

        /// Compile expression to a string with C code
        [[nodiscard]] std::string c_code() const;

        /// Save expression as code to a file
        void save_c_code(std::string_view file_name) const;

        /// Compile the expression with a C compiler and return a function
        /// pointer
        [[nodiscard]] node_lambda compile() const;

      public /* operators */:
        /*
         * Most other operators are defined in
         * functions/operators.h
         */

        /// Attribution operator from node_interface
        sym &operator=(const node_interface &);

        sym &operator=(boolean &&);

        sym &operator=(integer &&);

        sym &operator=(real &&);

        sym &operator=(rational &&);

        sym &operator=(variable &&);

        sym &operator=(constant &&);

        sym &operator=(summation &&);

        sym &operator=(product &&);

        sym &operator=(abs &&);

        sym &operator=(cos &&);

        sym &operator=(cosh &&);

        sym &operator=(log &&);

        sym &operator=(pow &&);

        sym &operator=(sin &&);

        sym &operator=(sinh &&);

        sym &operator=(statement &&);

        /// Attribution operator from sym
        sym &operator=(const sym &);

        /// Attribution operator from rvalue sym
        sym &operator=(sym &&) noexcept;

        /// Attribution operator from number interface
        sym &operator=(const number_interface &);

        /// Attribution operator from int
        sym &operator=(bool);

        /// Attribution operator from int
        sym &operator=(int);

        /// Attribution operator from double
        sym &operator=(double);

        /// Attribution operator from a string
        sym &operator=(std::string_view);

        /// Element access from size_t for internal nodes
        const sym &operator[](size_t) const;

        /// Element access from size_t for internal nodes
        sym &operator[](size_t);

        /// Element access from size_t for internal nodes
        const sym &operator()(size_t) const;

        /// Element access from size_t for internal nodes
        sym &operator()(size_t);

        /// Change commutative
        sym operator~() const;

        /// Convert to bool
        explicit operator bool() const;

        /// Convert to int
        explicit operator int() const;

        /// Convert to double
        explicit operator double() const;

      public /* check type */:
        /// Return type of this symbolic variable
        [[nodiscard]] const std::type_info &type() const;

        /// Return true if this is a terminal node
        bool is_terminal() const;

        /// Return true if this is a terminal node
        bool is_internal() const;

        /// Check if symbol is numeric
        [[nodiscard]] bool is_number() const;

        /// Check if symbol is int
        [[nodiscard]] bool is_boolean_number() const;

        /// Check if symbol is int
        [[nodiscard]] bool is_integer_number() const;

        /// Check if symbol is double
        [[nodiscard]] bool is_real_number() const;

        /// Check if symbol is rational
        [[nodiscard]] bool is_rational_number() const;

        /// Check if symbol is zero (and numeric)
        [[nodiscard]] bool is_zero() const;

        /// Check if symbol is one (and numeric)
        [[nodiscard]] bool is_one() const;

        /// Check if symbol is variable
        [[nodiscard]] bool is_variable() const;

        /// Check if symbol is bool_variable
        [[nodiscard]] bool is_boolean_variable() const;

        /// Check if symbol is summation
        /// Check if symbol is int_variable
        [[nodiscard]] bool is_integer_variable() const;

        /// Check if symbol is double_variable
        [[nodiscard]] bool is_real_variable() const;

        /// Check if symbol is double_variable
        [[nodiscard]] bool is_constant() const;

        /// Check if symbol is summation
        [[nodiscard]] bool is_summation() const;

        /// Check if symbol is product
        [[nodiscard]] bool is_product() const;

        /// Check if symbol is function
        [[nodiscard]] bool is_function() const;

        /// Check if symbol is statement
        [[nodiscard]] bool is_statement() const;

        /// Check if symbol is equation
        [[nodiscard]] bool is_equation() const;

        /// Check if symbol is inequality
        [[nodiscard]] bool is_inequality() const;

        /// Check if symbol is of a given type
        template <class NODE_TYPE> [[nodiscard]] bool is() const {
            return this->type() == typeid(NODE_TYPE);
        }

        /// Check if symbol inherits from a given type
        template <class NODE_TYPE> [[nodiscard]] bool inherits_from() const {
            auto p = root_node_as<NODE_TYPE>();
            return p != nullptr;
        }

      public /* non-modifying functions */:
        /// Get a shared pointer to the root node
        [[nodiscard]] std::shared_ptr<const node_interface> root_node() const;

        std::shared_ptr<node_interface> root_node();

        /// Get a shared pointer to the root node as another pointer type
        template <class DERIVED_NODE_TYPE>
        [[nodiscard]] std::shared_ptr<const DERIVED_NODE_TYPE>
        root_node_as() const {
            return std::dynamic_pointer_cast<const DERIVED_NODE_TYPE>(
                this->root_node());
        }

        template <class DERIVED_NODE_TYPE>
        std::shared_ptr<DERIVED_NODE_TYPE> root_node_as() {
            return std::dynamic_pointer_cast<DERIVED_NODE_TYPE>(
                this->root_node());
        }

      private:
        /// Parent node
        std::shared_ptr<node_interface> root_node_;
    };

} // namespace sympp

#endif
