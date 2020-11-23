// C++
#include <fstream>
#include <iostream>
#include <memory>
#include <string_view>
#include <typeinfo>

// TinyCC
#include <tcc/libtcc_ext.h>

// Internal
#include <sympp/core/node_interface.h>
#include <sympp/core/sym.h>
#include <sympp/core/sym_error.h>
#include <sympp/node/function/abs.h>
#include <sympp/node/function/cos.h>
#include <sympp/node/function/cosh.h>
#include <sympp/node/function/log.h>
#include <sympp/node/function/pow.h>
#include <sympp/node/function/sin.h>
#include <sympp/node/function/sinh.h>
#include <sympp/node/operation/product.h>
#include <sympp/node/operation/summation.h>
#include <sympp/node/statement/statement.h>
#include <sympp/node/terminal/boolean.h>
#include <sympp/node/terminal/constant.h>
#include <sympp/node/terminal/integer.h>
#include <sympp/node/terminal/rational.h>
#include <sympp/node/terminal/real.h>
#include <sympp/node/terminal/variable.h>

namespace sympp {

    sym::sym() : sym(integer(0)) {}

    sym::sym(const sym &s) : root_node_(s.root_node_->clone()) {}

    sym::sym(sym &&s) noexcept : root_node_(std::move(s.root_node_)) {}

    sym::sym(const node_interface &s) : root_node_(s.clone()) {}

    sym::sym(boolean &&s) : root_node_(new boolean(std::move(s))) {}

    sym::sym(integer &&s) : root_node_(new integer(std::move(s))) {}

    sym::sym(real &&s) : root_node_(new real(std::move(s))) {}

    sym::sym(rational &&s) : root_node_(new rational(std::move(s))) {}

    sym::sym(variable &&s) : root_node_(new variable(std::move(s))) {}

    sym::sym(constant &&s) : root_node_(new constant(std::move(s))) {}

    sym::sym(summation &&s) : root_node_(new summation(std::move(s))) {}

    sym::sym(product &&s) : root_node_(new product(std::move(s))) {}

    sym::sym(abs &&s) : root_node_(new abs(std::move(s))) {}

    sym::sym(cos &&s) : root_node_(new cos(std::move(s))) {}

    sym::sym(cosh &&s) : root_node_(new cosh(std::move(s))) {}

    sym::sym(log &&s) : root_node_(new log(std::move(s))) {}

    sym::sym(pow &&s) : root_node_(new pow(std::move(s))) {}

    sym::sym(sin &&s) : root_node_(new sin(std::move(s))) {}

    sym::sym(sinh &&s) : root_node_(new sinh(std::move(s))) {}

    sym::sym(statement &&s) : root_node_(new statement(std::move(s))) {}

    sym::sym(bool b) : root_node_(new boolean(b)) {}

    sym::sym(int i) : root_node_(new integer(i)) {}

    sym::sym(double d) : root_node_(new real(d)) {}

    sym::sym(std::string_view s, const number_interface &v)
        : root_node_(new constant(s, sym(v))) {}
    sym::sym(std::string_view s, bool v) : root_node_(new constant(s, v)) {}
    sym::sym(std::string_view s, int v) : root_node_(new constant(s, v)) {}
    sym::sym(std::string_view s, double v) : root_node_(new constant(s, v)) {}
    sym::sym(std::string_view s, const sym &v)
        : root_node_(new constant(s, v)) {}

    sym::sym(std::string_view s)
        : root_node_(new variable(s, numeric_type::var_real)) {}

    sym::sym(std::string_view s, numeric_type n)
        : root_node_(new variable(s, n)) {}

    sym::sym(numeric_type n, std::string_view s)
        : root_node_(new variable(n, s)) {}

    sym::~sym() = default;

    sym &sym::simplify() {
        this->root_node_->simplify();
        return *this;
    }

    sym &sym::simplify(double ratio) {
        this->root_node_->simplify(ratio);
        return *this;
    }

    sym &sym::simplify(double ratio, complexity_lambda func) {
        this->root_node_->simplify(ratio, func);
        return *this;
    }

    /// Put polynomial into a canonical form of a sum of monomials
    sym &sym::expand() {
        std::optional<sym> r = this->root_node_->expand();
        if (r) {
            *this = std::move(*r);
        }
        return *this;
    }

    /// Takes a polynomial and factors it into irreducible factors
    sym &sym::factor() {
        std::optional<sym> r = this->root_node_->factor();
        if (r) {
            *this = std::move(*r);
        }
        return *this;
    }

    /// Collects common powers of a term in an expression
    sym &sym::collect() {
        std::optional<sym> r = this->root_node_->collect();
        if (r) {
            *this = std::move(*r);
        }
        return *this;
    }

    /// Take any rational function and put it into the standard canonical
    /// form
    sym &sym::cancel() {
        std::optional<sym> r = this->root_node_->cancel();
        if (r) {
            *this = std::move(*r);
        }
        return *this;
    }

    /// Performs a partial fraction decomposition on a rational function
    sym &sym::appart() {
        std::optional<sym> r = this->root_node_->appart();
        if (r) {
            *this = std::move(*r);
        }
        return *this;
    }

    /// Simplify expressions using trigonometric identities
    sym &sym::trigsimp() {
        std::optional<sym> r = this->root_node_->trigsimp();
        if (r) {
            *this = std::move(*r);
        }
        return *this;
    }

    /// Expand trigonometric functions, that is, apply the sum or double
    /// angle identities
    sym &sym::expand_trig() {
        std::optional<sym> r = this->root_node_->expand_trig();
        if (r) {
            *this = std::move(*r);
        }
        return *this;
    }

    /// Applies identities x^ax^b=x^{a+b} and x^ay^a=(xy)^a from left to
    /// right Identity x^ay^a=(xy)^a is true if at least x and y are
    /// non-negative and a is real.
    sym &sym::powsimp() {
        std::optional<sym> r = this->root_node_->powsimp();
        if (r) {
            *this = std::move(*r);
        }
        return *this;
    }

    /// Applies identity x^ax^b=x^{a+b} from right to left
    sym &sym::expand_power_exp() {
        std::optional<sym> r = this->root_node_->expand_power_exp();
        if (r) {
            *this = std::move(*r);
        }
        return *this;
    }

    /// Applies identity x^ay^a=(xy)^a from right to left
    /// Identity x^ay^a=(xy)^a is true if at least x and y are non-negative
    /// and a is real.
    sym &sym::expand_power_base() {
        std::optional<sym> r = this->root_node_->expand_power_base();
        if (r) {
            *this = std::move(*r);
        }
        return *this;
    }

    /// Applies identity x^a^b=x^{ab} from left to right
    /// Identity x^a^b=x^{ab} is true if b is an integer
    sym &sym::powdenest() {
        std::optional<sym> r = this->root_node_->powdenest();
        if (r) {
            *this = std::move(*r);
        }
        return *this;
    }

    /// Apply identities log(xy)=log(x)+log(y) and log(x^n)=nlog(x) from
    /// left to right Only if x and y are positive and n is real.
    sym &sym::expand_log() {
        std::optional<sym> r = this->root_node_->expand_log();
        if (r) {
            *this = std::move(*r);
        }
        return *this;
    }

    /// Apply identities log(xy)=log(x)+log(y) and log(x^n)=nlog(x) from
    /// right to left Only if x and y are positive and n is real.
    sym &sym::logcombine() {
        std::optional<sym> r = this->root_node_->logcombine();
        if (r) {
            *this = std::move(*r);
        }
        return *this;
    }

    void sym::stream(std::ostream &o, bool print_format_symbolic) const {
        this->root_node_->stream(o, print_format_symbolic);
    }

    sym &sym::subs(const sym &x, const sym &y) {
        std::optional<sym> r = this->root_node_->subs(x, y);
        if (r) {
            *this = std::move(*r);
        }
        return *this;
    }

    sym &sym::subs(const sym &x, bool b) { return subs(x, sym(boolean(b))); }

    sym &sym::subs(const sym &x, int i) { return subs(x, sym(integer(i))); }

    sym &sym::subs(const sym &x, double d) { return subs(x, sym(real(d))); }

    sym &sym::subs(const sym &e) {
        if (e.is_statement()) {
            return subs(*e.root_node_as<statement>());
        }
        return *this;
    }

    sym &sym::subs(const statement &e) { return subs(e.lhs(), e.rhs()); }

    sym &sym::subs(const std::vector<statement> &l) {
        for (const auto &s : l) {
            this->subs(s);
        }
        return *this;
    }

    int sym::compare(const sym &s) const {
        return this->root_node_->compare(*s.root_node());
    }

    int sym::compare(const node_interface &s) const {
        return this->root_node_->compare(s);
    }

    sym sym::coeff(const sym &s) const { return this->coeff(*s.root_node()); }
    sym sym::coeff(const node_interface &s) const {
        return this->root_node_->coeff(s);
    }
    sym sym::coeff(const sym &s, int i) const {
        if (i == 0) {
            sym s2 = *this;
            s2.subs(s, 0);
            return s2;
        }
        return root_node_->coeff(pow(s, sym(integer(i))));
    }
    sym sym::coeff(int i) const { return coeff(integer(i)); }
    sym sym::coeff(double d) const { return coeff(real(d)); }

    bool sym::is_commutative() const {
        return this->root_node_->is_commutative();
    }

    size_t sym::size() const { return this->root_node_->size(); }

    size_t sym::count_ops() const { return this->root_node_->count_ops(); }

    function_type sym::calculate_function_type() {
        if (is_number() || is_variable() || is_constant()) {
            return function_type::linear;
        }

        auto is_special_non_linear = [](const sym &s) {
            return s.is_function();
        };

        if (is_special_non_linear(*this)) {
            return function_type::nonlinear;
        } else if (is_product()) {
            bool there_is_one_variable = false;
            auto i = begin();
            while (i != end()) {
                if (is_special_non_linear(*i)) {
                    return function_type::nonlinear;
                }
                if (i->is_variable()) {
                    if (there_is_one_variable) {
                        return function_type::nonlinear;
                    } else {
                        there_is_one_variable = true;
                    }
                }
                if (i->is_function()) {
                    return function_type::nonlinear;
                }
                ++i;
            }
            return function_type::linear;
        }

        if (is_summation()) {
            auto i = begin();
            while (i != end()) {
                function_type currentt = i->calculate_function_type();
                if (currentt == function_type::nonlinear) {
                    return function_type::nonlinear;
                }
                ++i;
            }
            return function_type::linear;
        }

        return function_type::unknown;
    }

    std::vector<sym>::iterator sym::begin() { return root_node_->begin(); }

    std::vector<sym>::iterator sym::end() { return root_node_->end(); }

    std::vector<sym>::const_iterator sym::begin() const {
        return root_node_->begin();
    }

    std::vector<sym>::const_iterator sym::end() const {
        return root_node_->end();
    }

    void sym::put_indexes() {
        std::unordered_map<int, sym> bool_symbols_vars;
        std::unordered_map<int, sym> int_symbols_vars;
        std::unordered_map<int, sym> real_symbols_vars;
        std::unordered_map<std::string, int> bool_symbols_names;
        std::unordered_map<std::string, int> int_symbols_names;
        std::unordered_map<std::string, int> real_symbols_names;
        this->root_node_->put_indexes(bool_symbols_vars, int_symbols_vars,
                                      real_symbols_vars, bool_symbols_names,
                                      int_symbols_names, real_symbols_names);
    }

    double sym::evaluate(const std::vector<uint8_t> &bool_values,
                         const std::vector<int> &int_values,
                         const std::vector<double> &double_values) const {
        return this->root_node_->evaluate(bool_values, int_values,
                                          double_values);
    }

    sym sym::evaluate_sym(const std::vector<uint8_t> &bool_values,
                          const std::vector<int> &int_values,
                          const std::vector<double> &double_values) const {
        return this->root_node_->evaluate_sym(bool_values, int_values,
                                              double_values);
    }

    node_lambda sym::lambdify() const { return this->root_node_->lambdify(); }

    std::string sym::c_code() const {
        std::string code;
        int sum_level = -1;
        int prod_level = -1;
        int function_level = 0;
        bool leaf = false;
        std::stack<std::string> code_aux;

        code +=
            // include the "Simple libc header for TCC"
            "extern double sin(double a);\n"
            "extern double cos(double a);\n"
            "extern double abs(double a);\n"
            "extern double pow(double a, double b);\n"
            "#ifdef _WIN32\n" // dynamically linked data needs 'dllimport'
            " __attribute__((dllimport))\n"
            "#endif\n"
            "\n";

        code += "double evaluate(_Bool bool_values[], int int_values[], double "
                "double_values[])\n"
                "{\n"
                " double sum[5]; \n"
                " double prod[5]; \n"
                " double evaluate = 0;\n";

        this->root_node_->c_code(function_level, sum_level, prod_level, leaf,
                                 code_aux, code);

        if (this->is_summation()) {

            code += " evaluate = sum[0];\n";
        }

        if (this->is_product()) {

            code += " evaluate = prod[0];\n";
        }

        code += " return evaluate;\n";
        code += "}\n";

        return code;
    }

    void sym::save_c_code(std::string_view file_name) const {
        std::string code = this->c_code();
        std::ofstream out;
        out.open(file_name.data());
        out << code;
        out.close();
    }

    /// Handle error function for TinyCC
    void handle_error(void *opaque, const char *msg) {
        fprintf(reinterpret_cast<FILE *>(opaque), "%s\n", msg);
    }

    /// Function wrapper mysin for TinyCC
    double mysin(double a) { return std::sin(a); }

    /// Function wrapper myabs for TinyCC
    double myabs(double a) { return std::abs(a); }

    /// Function wrapper mycos for TinyCC
    double mycos(double a) { return std::cos(a); }

    /// Function wrapper mypow for TinyCC
    double mypow(double a, double b) { return std::pow(a, b); }

    /// Function wrapper add for TinyCC
    int add(int a, int b) { return a + b; }

    node_lambda sym::compile() const {
        std::string code = this->c_code();

        TCCState *s = atcc_new();
        if (!s || tcc_get_error_func(s) != nullptr ||
            tcc_get_error_opaque(s) != nullptr) {
            throw std::runtime_error("Could not create tcc state");
        }

        tcc_set_error_func(s, stderr, handle_error);
        if (tcc_get_error_func(s) != handle_error &&
            tcc_get_error_opaque(s) != stderr) {
            throw std::runtime_error("Could not set tcc error function");
        }

        /* MUST BE CALLED before any compilation */
        tcc_set_output_type(s, TCC_OUTPUT_MEMORY);
        if (tcc_compile_string(s, code.c_str()) == -1) {
            throw std::runtime_error("Could not compile C code string");
        }

        /* Add symbols that the compiled program can use. */
        auto sin_pointer = &mysin;
        void *sin_void = reinterpret_cast<void *&>(sin_pointer);
        tcc_add_symbol(s, "sin", sin_void);

        auto cos_pointer = &mycos;
        void *cos_void = reinterpret_cast<void *&>(cos_pointer);
        tcc_add_symbol(s, "cos", cos_void);

        auto pow_pointer = &mypow;
        void *pow_void = reinterpret_cast<void *&>(pow_pointer);
        tcc_add_symbol(s, "pow", pow_void);

        auto abs_pointer = &myabs;
        void *abs_void = reinterpret_cast<void *&>(abs_pointer);
        tcc_add_symbol(s, "abs", abs_void);

        /* relocate the code */
        if (tcc_relocate(s, TCC_RELOCATE_AUTO) < 0) {
            throw std::runtime_error("Could not relocate C code");
        }

        /* get entry symbol */
        double (*func)(const uint8_t[], const int[], const double[]);
        func = reinterpret_cast<double (*)(const uint8_t *, const int *,
                                           const double *)>(
            tcc_get_symbol(s, "evaluate"));

        if (!func) {
            throw std::runtime_error(
                "Could not get the evaluation symbol from C code");
        }

        /* delete the state */
        tcc_delete(s);
        return [func](const std::vector<uint8_t> &bools,
                      const std::vector<int> &ints,
                      const std::vector<double> &doubles) -> double {
            return func(bools.data(), ints.data(), doubles.data());
        };
    }

    sym &sym::operator=(const node_interface &s) {
        root_node_.reset(s.clone());
        return *this;
    }

    sym &sym::operator=(boolean &&s) {
        root_node_ = std::make_shared<boolean>((std::move(s)));
        return *this;
    }

    sym &sym::operator=(integer &&s) {
        root_node_ = std::make_shared<integer>((std::move(s)));
        return *this;
    }

    sym &sym::operator=(real &&s) {
        root_node_ = std::make_shared<real>((std::move(s)));
        return *this;
    }

    sym &sym::operator=(rational &&s) {
        root_node_ = std::make_shared<rational>((std::move(s)));
        return *this;
    }

    sym &sym::operator=(variable &&s) {
        root_node_ = std::make_shared<variable>((std::move(s)));
        return *this;
    }

    sym &sym::operator=(constant &&s) {
        root_node_ = std::make_shared<constant>((std::move(s)));
        return *this;
    }

    sym &sym::operator=(summation &&s) {
        root_node_ = std::make_shared<summation>((std::move(s)));
        return *this;
    }

    sym &sym::operator=(product &&s) {
        root_node_ = std::make_shared<product>((std::move(s)));
        return *this;
    }

    sym &sym::operator=(abs &&s) {
        root_node_ = std::make_shared<abs>((std::move(s)));
        return *this;
    }

    sym &sym::operator=(cos &&s) {
        root_node_ = std::make_shared<cos>((std::move(s)));
        return *this;
    }

    sym &sym::operator=(cosh &&s) {
        root_node_ = std::make_shared<cosh>((std::move(s)));
        return *this;
    }

    sym &sym::operator=(log &&s) {
        root_node_ = std::make_shared<log>((std::move(s)));
        return *this;
    }

    sym &sym::operator=(pow &&s) {
        root_node_ = std::make_shared<pow>((std::move(s)));
        return *this;
    }

    sym &sym::operator=(sin &&s) {
        root_node_ = std::make_shared<sin>((std::move(s)));
        return *this;
    }

    sym &sym::operator=(sinh &&s) {
        root_node_ = std::make_shared<sinh>((std::move(s)));
        return *this;
    }

    sym &sym::operator=(statement &&s) {
        root_node_ = std::make_shared<statement>((std::move(s)));
        return *this;
    }

    sym &sym::operator=(const sym &s) {
        if (&s != this) {
            root_node_.reset(s.root_node_->clone());
        }
        return *this;
    }

    sym &sym::operator=(sym &&s) noexcept {
        if (&s != this) {
            root_node_ = std::move(s.root_node_);
        }
        return *this;
    }

    sym &sym::operator=(const number_interface &s) {
        root_node_.reset(s.clone());
        return *this;
    }

    sym &sym::operator=(bool i) { return *this = integer(i); }

    sym &sym::operator=(int i) { return *this = integer(i); }

    sym &sym::operator=(double d) { return *this = real(d); }

    sym &sym::operator=(std::string_view s) { return *this = sym(s); }

    const sym &sym::operator[](size_t idx) const { return begin()[idx]; }

    sym &sym::operator[](size_t idx) { return begin()[idx]; }

    const sym &sym::operator()(size_t idx) const { return begin()[idx]; }

    sym &sym::operator()(size_t idx) { return begin()[idx]; }

    sym sym::operator~() const {
        if (is_function()) {
            sym s2 = *this;
            auto f = s2.root_node_as<function_interface>();
            f->is_commutative(!f->is_commutative());
            return s2;
        }
        return *this;
    }

    sym::operator bool() const {
        if (is_number() || is_constant()) {
            auto f = root_node_as<number_interface>();
            f->operator bool();
        }
        throw sym_error(sym_error::NotNumeric);
    }

    sym::operator int() const {
        if (is_number() || is_constant()) {
            auto f = root_node_as<number_interface>();
            f->operator int();
        }
        throw sym_error(sym_error::NotNumeric);
    }

    sym::operator double() const {
        if (is_number() || is_constant()) {
            auto f = root_node_as<number_interface>();
            f->operator double();
        }
        throw sym_error(sym_error::NotNumeric);
    }

    const std::type_info &sym::type() const { return this->root_node_->type(); }

    bool sym::is_terminal() const { return this->root_node_->is_terminal(); }

    bool sym::is_internal() const { return !this->root_node_->is_terminal(); }

    bool sym::is_number() const { return inherits_from<number_interface>(); }

    bool sym::is_rational_number() const { return type() == typeid(rational); }

    bool sym::is_boolean_number() const { return type() == typeid(boolean); }

    bool sym::is_integer_number() const { return type() == typeid(integer); }

    bool sym::is_real_number() const { return type() == typeid(real); }

    bool sym::is_zero() const {
        if (is_number()) {
            auto n = root_node_as<number_interface>();
            return n->is_zero();
        } else if (is_constant()) {
            auto c = root_node_as<constant>();
            return c->is_zero();
        }
        throw sym_error(sym_error::NotNumeric);
    }

    bool sym::is_one() const {
        if (is_number()) {
            auto n = root_node_as<number_interface>();
            return n->is_one();
        } else if (is_constant()) {
            auto c = root_node_as<constant>();
            return c->is_one();
        }
        throw sym_error(sym_error::NotNumeric);
    }

    bool sym::is_variable() const { return type() == typeid(variable); }

    bool sym::is_boolean_variable() const {
        if (is_variable()) {
            auto n = root_node_as<variable>();
            return n->num_type() == numeric_type::var_boolean;
        }
        return false;
    }

    bool sym::is_integer_variable() const {
        if (is_variable()) {
            auto n = root_node_as<variable>();
            return n->num_type() == numeric_type::var_integer;
        }
        return false;
    }

    bool sym::is_real_variable() const {
        if (is_variable()) {
            auto n = root_node_as<variable>();
            return n->num_type() == numeric_type::var_real;
        }
        return false;
    }

    bool sym::is_constant() const { return type() == typeid(constant); }

    bool sym::is_summation() const { return type() == typeid(summation); }

    bool sym::is_product() const { return type() == typeid(product); }

    bool sym::is_function() const {
        return inherits_from<function_interface>();
    }

    bool sym::is_statement() const { return type() == typeid(statement); }

    bool sym::is_equation() const {
        if (is_statement()) {
            auto s = root_node_as<statement>();
            return s->statement_enum_type() ==
                   statement::statement_type::equality;
        }
        return false;
    }

    bool sym::is_inequality() const {
        if (is_statement()) {
            auto s = root_node_as<statement>();
            return s->statement_enum_type() !=
                   statement::statement_type::equality;
        }
        return false;
    }

    std::shared_ptr<const node_interface> sym::root_node() const {
        return root_node_;
    }

    std::shared_ptr<node_interface> sym::root_node() { return root_node_; }

} // namespace sympp
