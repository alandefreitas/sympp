//
// Created by Alan Freitas on 20/09/20.
//

#include <numeric>

#include "rational.h"
#include <sympp/core/sym_error.h>
#include <sympp/node/function/pow.h>
#include <sympp/node/operation/product.h>
#include <sympp/node/operation/summation.h>
#include <sympp/node/terminal/boolean.h>
#include <sympp/node/terminal/integer.h>
#include <sympp/node/terminal/real.h>

namespace sympp {

    rational::rational() : numerator_(0), denominator_(1) {}
    rational::rational(const rational &v)
        : numerator_(v.numerator_), denominator_(v.denominator_) {}
    rational &rational::operator=(const rational &v) {
        numerator_ = v.numerator_;
        denominator_ = v.denominator_;
        return *this;
    }
    rational::rational(rational &&v) noexcept
        : numerator_(v.numerator_), denominator_(v.denominator_) {}
    rational &rational::operator=(rational &&v) {
        numerator_ = v.numerator_;
        denominator_ = v.denominator_;
        return *this;
    }
    rational::rational(bool v)
        : numerator_(static_cast<int>(v)), denominator_(1) {}
    rational::rational(int v) : numerator_(v), denominator_(1) {}
    rational::rational(int n, int d) : numerator_(n), denominator_(d) {
        if (d == 0) {
            std::cerr << "Zero denominator in rational number " << std::endl;
        }
        normalize();
    }
    rational::rational(double v) : numerator_(0), denominator_(1) {
        // https://stackoverflow.com/questions/95727/how-to-convert-floats-to-human-readable-fractions
        long m[2][2] = {{1, 0}, {0, 1}};
        double x = v;
        double startx = v;
        const long maxden = 70000;
        long ai = static_cast<long>(x);
        // loop finding terms until denominator gets too big
        while (m[1][0] * ai + m[1][1] <= maxden) {
            // matrix operation
            long tmp = m[0][0] * ai + m[0][1];
            m[0][1] = m[0][0];
            m[0][0] = tmp;
            tmp = m[1][0] * ai + m[1][1];
            m[1][1] = m[1][0];
            m[1][0] = tmp;

            // AF: division by zero would happen
            auto dai = static_cast<double>(ai);
            if (x == dai) {
                break;
            }
            x = 1 / (x - dai);

            // AF: representation failure;
            constexpr auto threshold = static_cast<double>(0x7FFFFFFF);
            if (x > threshold) {
                break;
            }

            // update ai for next iteration
            ai = static_cast<long>(x);
        }

        // now remaining x is between 0 and 1/ai
        // approx as either 0 or 1/m where m is max that will fit in maxden
        // first try zero
        double finalx =
            static_cast<double>(m[0][0]) / static_cast<double>(m[1][0]);
        const double error1 = startx - finalx;
        numerator_ = static_cast<int>(m[0][0]);
        denominator_ = static_cast<int>(m[1][0]);

        // now try the other possibility
        ai = (maxden - m[1][1]) / m[1][0];
        m[0][0] = m[0][0] * ai + m[0][1];
        m[1][0] = m[1][0] * ai + m[1][1];
        finalx = static_cast<double>(m[0][0]) / static_cast<double>(m[1][0]);
        const double error2 = startx - finalx;
        if (error2 < error1) {
            numerator_ = static_cast<int>(m[0][0]);
            denominator_ = static_cast<int>(m[1][0]);
        }
    }

    rational::rational(const number_interface &v) : rational() {
        if (v.type() == typeid(rational)) {
            const auto &p = dynamic_cast<const rational &>(v);
            *this = p;
        } else {
            *this = rational(static_cast<int>(v));
        }
    }

    rational::rational(const node_interface &v)
        : rational(dynamic_cast<const number_interface &>(v)) {}
    rational::rational(const sym &v) : rational(*v.root_node()) {}
    rational::~rational() = default;

    void rational::normalize() {
        if (denominator_ < 0) {
            numerator_ = -numerator_;
            denominator_ = -denominator_;
        }
        int t = numerator_ < 0 ? -numerator_ : numerator_;
        t = std::gcd(t, denominator_);
        if (t > 1) {
            numerator_ /= t;
            denominator_ /= t;
        }
    }

    double rational::evaluate(const std::vector<uint8_t> &,
                              const std::vector<int> &,
                              const std::vector<double> &) const {
        return static_cast<double>(*this);
    }

    sym rational::evaluate_sym(const std::vector<uint8_t> &,
                               const std::vector<int> &,
                               const std::vector<double> &) const {
        return sym(*this);
    }

    node_lambda rational::lambdify() const {
        // the lambda version of this returns this integer as a constant
        auto d = static_cast<double>(*this);
        return [d](const std::vector<uint8_t> &, const std::vector<int> &,
                   const std::vector<double> &) -> double { return d; };
    }

    void rational::c_code(int &, int &, int &, bool &leaf,
                          std::stack<std::string> &code_aux,
                          std::string &code) const {
        auto d = static_cast<double>(*this);
        if (leaf) {
            code_aux.push(std::to_string(d));
        } else {
            code += std::to_string(d);
        }
    }

    void rational::stream(std::ostream &o, bool print_format_symbolic) const {
        if (print_format_symbolic) {
            o << numerator_ << "/" << denominator_;
        } else {
            o << static_cast<double>(*this);
        }
    }

    const std::type_info &rational::type() const {
        return typeid(decltype(*this));
    }

    node_interface *rational::clone() const {
        return dynamic_cast<node_interface *>(new rational(*this));
    }

    const std::type_info &rational::numeric_type() const {
        return typeid(std::pair<int, int>);
    }

    bool rational::is_zero() const { return numerator_ == 0; }

    bool rational::is_one() const { return numerator_ == denominator_; }

    bool rational::is_negative() const { return numerator_ < 0; }

    int rational::compare_number(const number_interface &rhs) const {
        if (rhs.type() == typeid(real)) {
            auto lhs_double = static_cast<double>(*this);
            auto rhs_double = static_cast<double>(rhs);
            if (lhs_double < rhs_double) {
                return -1;
            } else if (lhs_double > rhs_double) {
                return +1;
            } else {
                return 0;
            }
        } else if (rhs.type() == typeid(integer)) {
            return rhs.compare(*this) * -1;
        } else if (rhs.type() == typeid(boolean)) {
            return rhs.compare(*this) * -1;
        } else if (rhs.type() == typeid(rational)) {
            auto &rhsr = dynamic_cast<const rational &>(rhs);
            int l = std::lcm(this->denominator_, rhsr.denominator_);
            int n1 = this->numerator_ * (l / this->denominator_);
            int n2 = rhsr.numerator_ * (l / rhsr.denominator_);
            if (n1 < n2) {
                return -1;
            } else if (n1 > n2) {
                return +1;
            } else {
                return 0;
            }
        }
        return 0;
    }

    rational::operator bool() const { return numerator_ != 0; }

    rational::operator int() const { return numerator_ / denominator_; }

    rational::operator double() const {
        return static_cast<double>(numerator_) /
               static_cast<double>(denominator_);
    }

    sym rational::add(const number_interface &rhs) const {
        if (rhs.type() == typeid(boolean) || rhs.type() == typeid(integer)) {
            // might promote the boolean
            auto rhs_int = static_cast<int>(rhs);
            return sym(
                rational(numerator_ + rhs_int * denominator_, denominator_));
        } else if (rhs.type() == typeid(rational)) {
            // might promote the boolean
            const auto &rhsr = dynamic_cast<const rational &>(rhs);
            int l = std::lcm(this->denominator_, rhsr.denominator_);
            int n1 = this->numerator_ * (l / this->denominator_);
            int n2 = rhsr.numerator_ * (l / rhsr.denominator_);
            return sym(rational(n1 + n2, l));
        } else {
            // Delegate commutative function to higher level (real)
            return rhs.add(*this);
        }
    }

    sym rational::sub(const number_interface &rhs) const {
        if (rhs.type() == typeid(boolean)) {
            auto rhs_bool = static_cast<bool>(rhs);
            if (rhs_bool) {
                return sym(rational(numerator_ - denominator_, denominator_));
            } else {
                return sym(*this);
            }
        } else if (rhs.type() == typeid(integer)) {
            auto rhs_int = static_cast<int>(rhs);
            return sym(
                rational(numerator_ - rhs_int * denominator_, denominator_));
        } else if (rhs.type() == typeid(real)) {
            auto lhs_double = static_cast<double>(*this);
            auto rhs_double = static_cast<double>(rhs);
            return sym(real(lhs_double - rhs_double));
        } else if (rhs.type() == typeid(rational)) {
            const auto &rhsr = dynamic_cast<const rational &>(rhs);
            int l = std::lcm(this->denominator_, rhsr.denominator_);
            int n1 = this->numerator_ * (l / this->denominator_);
            int n2 = rhsr.numerator_ * (l / rhsr.denominator_);
            return sym(rational(n1 - n2, l));
        } else {
            return sym(summation(sym(*this), sym(product(sym(-1), sym(rhs)))));
        }
    }

    sym rational::mul(const number_interface &rhs) const {
        if (rhs.type() == typeid(boolean) || rhs.type() == typeid(integer)) {
            auto rhs_int = static_cast<int>(rhs);
            return sym(rational(numerator_ * rhs_int, denominator_));
        } else if (rhs.type() == typeid(rational)) {
            auto rhsr = static_cast<rational>(rhs);
            return sym(rational(numerator_ * rhsr.numerator_,
                                denominator_ * rhsr.denominator_));
        } else {
            // Delegate commutative op
            return rhs.mul(*this);
        }
    }

    sym rational::div(const number_interface &rhs) const {
        if (rhs.type() == typeid(boolean)) {
            auto rhs_bool = static_cast<bool>(rhs);
            if (!rhs_bool) {
                return sym(integer(std::numeric_limits<int>::infinity()));
            } else {
                return sym(*this);
            }
        } else if (rhs.type() == typeid(integer)) {
            auto rhs_int = static_cast<int>(rhs);
            return sym(rational(numerator_, denominator_ * rhs_int));
        } else if (rhs.type() == typeid(real)) {
            auto lhs_double = static_cast<double>(*this);
            auto rhs_double = static_cast<double>(rhs);
            return sym(real(lhs_double / rhs_double));
        } else if (rhs.type() == typeid(rational)) {
            const auto &r = dynamic_cast<const rational &>(rhs);
            return sym(rational(numerator_ * r.denominator_,
                                denominator_ * r.numerator_));
        } else {
            return sym(product(sym(*this), sym(sympp::pow(sym(rhs), sym(-1)))));
        }
    }

    sym rational::mod(const number_interface &rhs) const {
        if (rhs.type() == typeid(boolean)) {
            auto rhs_bool = static_cast<bool>(rhs);
            if (!rhs_bool) {
                return sym(rational(numerator_, 0));
            } else {
                return sym(integer(0));
            }
        } else if (rhs.type() == typeid(integer)) {
            auto rhs_int = static_cast<int>(rhs);
            return sym(
                rational(numerator_ % (rhs_int * denominator_), denominator_));
        } else if (rhs.type() == typeid(real)) {
            auto lhs_double = static_cast<double>(*this);
            auto rhs_double = static_cast<double>(rhs);
            return sym(real(std::fmod(lhs_double, rhs_double)));
        } else if (rhs.type() == typeid(rational)) {
            const auto &rhsr = dynamic_cast<const rational &>(rhs);
            int l = std::lcm(this->denominator_, rhsr.denominator_);
            int n1 = this->numerator_ * (l / this->denominator_);
            int n2 = rhsr.numerator_ * (l / rhsr.denominator_);
            return sym(rational(n1 % n2, l));
        } else {
            throw sym_error(sym_error::NotNumeric);
        }
    }

    sym rational::zero() { return sym(rational(0, 1)); }

    sym rational::one() { return sym(rational(1, 1)); }

    int rational::numerator() const { return numerator_; }

    int rational::denominator() const { return denominator_; }

    rational rational::frac() const {
        rational temp(*this);
        if (temp.numerator_ < 0) {
            while (temp.numerator_ < 0) {
                // x += x + 1
                temp.numerator_ += denominator_;
            }
            temp.numerator_ -= denominator_;
        } else {
            while (0 < temp.numerator_) {
                temp.numerator_ -= denominator_;
            }
            temp.numerator_ += denominator_;
        }
        return temp;
    }

    std::optional<sym> rational::simplify(double, complexity_lambda) {
        if (denominator_ == 1) {
            return sym(integer(numerator_));
        } else {
            return std::nullopt;
        }
    }

} // namespace sympp