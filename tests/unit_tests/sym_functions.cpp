//
// Created by thiago on 08/10/20.
//

#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>
#include <sympp/node/function/abs.h>
#include <sympp/node/function/sinh.h>
#include <sympp/node/terminal/rational.h>
#include <sympp/sympp.h>

/*
TEST_CASE("Abs") {

    std::vector<double> doub_val(1, 1.0);
    std::vector<int> int_val(1, 1);
    std::vector<uint8_t> bool_val(1, false);

    using namespace sympp;
    sym n(integer(10));
    sym m(integer(-10));
    sym x(real(-10.5));
    sym ratio3 = sym(rational(-150, 15));

    sym y(true);

    sym modN = sym(sympp::abs(n));
    sym modM = sym(sympp::abs(m));
    sym modX = sym(sympp::abs(x));
    sym modBool = sym(sympp::abs(y));
    sym modRatio = sym(sympp::abs(ratio3));

    REQUIRE(modX < modN);
    REQUIRE(modM != modN);

    REQUIRE(modX.evaluate(bool_val, int_val, doub_val) >
            modN.evaluate(bool_val, int_val, doub_val));
    REQUIRE(modN.evaluate(bool_val, int_val, doub_val) ==
            modM.evaluate(bool_val, int_val, doub_val));

    REQUIRE(modM.evaluate_sym(bool_val, int_val, doub_val) == sym(integer(10)));
    REQUIRE(modM.evaluate_sym(bool_val, int_val, doub_val).simplify() == 10);
    REQUIRE(modBool.evaluate_sym(bool_val, int_val, doub_val).simplify() == 1);
    REQUIRE(modRatio.evaluate_sym(bool_val, int_val, doub_val).simplify() ==
            10);
}

TEST_CASE("Cos") {

    std::vector<double> doub_val(1, 1.0);
    std::vector<int> int_val(1, 1);
    std::vector<uint8_t> bool_val(1, false);

    using namespace sympp;
    sym A(integer(90));
    sym B(integer(180));
    sym C(integer(91));
    sym D(integer(90));

    sym cosA = sym(sympp::cos(A));
    sym cosB = sym(sympp::cos(B));
    sym cosC = sym(sympp::cos(C));
    sym cosD = sym(sympp::cos(D));

    REQUIRE(cosA < cosB);
    REQUIRE(cosA == cosD);
    REQUIRE(cosC != cosD);

    REQUIRE(cosC.evaluate(bool_val, int_val, doub_val) <
            cosD.evaluate(bool_val, int_val, doub_val));

    REQUIRE(cosC.evaluate_sym(bool_val, int_val, doub_val) == cosC);

    // REQUIRE(cosC.evaluate_sym(bool_val,int_val,doub_val).simplify() ==
    // -0.994367);
}

TEST_CASE("Cosh") {

    std::vector<double> doub_val(1, 1.0);
    std::vector<int> int_val(1, 1);
    std::vector<uint8_t> bool_val(1, false);

    using namespace sympp;
    sym A(integer(90));
    sym B(integer(180));
    sym C(integer(91));
    sym D(integer(90));

    sym coshA = sym(sympp::cosh(A));
    sym coshB = sym(sympp::cosh(B));
    sym coshC = sym(sympp::cosh(C));
    sym coshD = sym(sympp::cosh(D));

    REQUIRE(coshA < coshB);
    REQUIRE(coshA == coshD);
    REQUIRE(coshC != coshD);

    REQUIRE(coshC.evaluate(bool_val, int_val, doub_val) >
            coshD.evaluate(bool_val, int_val, doub_val));

    REQUIRE(coshC.evaluate_sym(bool_val, int_val, doub_val) == coshC);

    // REQUIRE(coshC.evaluate_sym(bool_val,int_val,doub_val).simplify() ==
    // -0.994367);
}

TEST_CASE("Sin") {

    std::vector<double> doub_val(1, 1.0);
    std::vector<int> int_val(1, 1);
    std::vector<uint8_t> bool_val(1, false);

    using namespace sympp;
    sym A(integer(90));
    sym B(integer(180));
    sym C(integer(91));
    sym D(integer(90));

    sym sinA = sym(sympp::sin(A));
    sym sinB = sym(sympp::sin(B));
    sym sinC = sym(sympp::sin(C));
    sym sinD = sym(sympp::sin(D));

    REQUIRE(sinA < sinB);
    REQUIRE(sinA == sinD);
    REQUIRE(sinC != sinD);

    REQUIRE(sinC.evaluate(bool_val, int_val, doub_val) <
            sinD.evaluate(bool_val, int_val, doub_val));

    REQUIRE(sinC.evaluate_sym(bool_val, int_val, doub_val) == sinC);

    // REQUIRE(sinC.evaluate_sym(bool_val,int_val,doub_val).simplify() ==
    // -0.994367);
}

TEST_CASE("Sinh") {

    std::vector<double> doub_val(1, 1.0);
    std::vector<int> int_val(1, 1);
    std::vector<uint8_t> bool_val(1, false);

    using namespace sympp;
    sym A(integer(90));
    sym B(integer(180));
    sym C(integer(91));
    sym D(integer(90));

    sym sinhA = sym(sympp::sinh(A));
    sym sinhB = sym(sympp::sinh(B));
    sym sinhC = sym(sympp::sinh(C));
    sym sinhD = sym(sympp::sinh(D));

    REQUIRE(sinhA < sinhB);
    REQUIRE(sinhA == sinhD);
    REQUIRE(sinhC != sinhD);

    REQUIRE(sinhC.evaluate(bool_val, int_val, doub_val) >
            sinhD.evaluate(bool_val, int_val, doub_val));

    REQUIRE(sinhC.evaluate_sym(bool_val, int_val, doub_val) == sinhC);

    // REQUIRE(sinhC.evaluate_sym(bool_val,int_val,doub_val).simplify() ==
    // -0.994367);
}


TEST_CASE("Pow") {

    std::vector<double> doub_val(1, 1.0);
    std::vector<int> int_val(1, 1);
    std::vector<uint8_t> bool_val(1, false);

    using namespace sympp;
    sym A(integer(90));
    sym B(integer(180));
    sym C(real(91.1));
    sym D(real(90.1));

    sym powA = sym(sympp::pow(A, B));
    sym powB = sym(sympp::pow(A, B));
    sym powC = sym(sympp::pow(C, C));
    sym powD = sym(sympp::pow(D, D));
    sym powE = sym(sympp::pow(A, constant::e()));

    REQUIRE(powA == powB);
    REQUIRE(powA != powC);

    REQUIRE(powA.evaluate(bool_val, int_val, doub_val) ==
            powB.evaluate(bool_val, int_val, doub_val));
    REQUIRE(powC.evaluate(bool_val, int_val, doub_val) >
            powD.evaluate(bool_val, int_val, doub_val));
    REQUIRE(powE.evaluate(bool_val, int_val, doub_val) <
            powC.evaluate(bool_val, int_val, doub_val));

    REQUIRE(powA.evaluate_sym(bool_val, int_val, doub_val) == powA);
}
*/

TEST_CASE("Log") {

    std::vector<double> doub_val(1, 1.0);
    std::vector<int> int_val(1, 1);
    std::vector<uint8_t> bool_val(1, false);

    using namespace sympp;
    sym A(integer(90));
    sym B(integer(180));
    sym C(integer(91));
    sym D(integer(90));

    sym logA = sym(sympp::log(A, B));
    sym logB = sym(sympp::log(A, B));
    sym logC = sym(sympp::log(C, C));
    sym logD = sym(sympp::log(D, D));

    // REQUIRE(logA == logB);
    // REQUIRE(logA != logC);

    // REQUIRE(logA.evaluate(bool_val, int_val, doub_val) ==
    // logB.evaluate(bool_val, int_val, doub_val));

    REQUIRE(logA.evaluate_sym(bool_val, int_val, doub_val) == logA);
}

TEST_CASE("Summation") {

    std::vector<double> doub_val(1, 1.0);
    std::vector<int> int_val(2, 1);
    std::vector<uint8_t> bool_val(1, false);

    using namespace sympp;
    sym A(integer(90));
    sym B(real(18.3));
    sym C(integer(-7));
    sym x = sym(variable(var_integer, "x"));
    sym x2 = sym(variable(var_integer, "x2"));

    sym cosA = sym(sympp::cos(A));
    sym absC = sym(sympp::abs(C));

    sym sum = A + absC;

    // REQUIRE(sum.is_summation());
    // REQUIRE(sum.evaluate(bool_val, int_val, doub_val) == 97);

    sum = x + x2 + B + C;
    sum.put_indexes();
    // REQUIRE(sum.evaluate(bool_val, int_val, doub_val) == 13.3);

    std::vector<sym> summands;
    summands.push_back(sym(integer(0)));
    summands.push_back(x.evaluate_sym(bool_val, int_val, doub_val));
    summands.push_back(x2.evaluate_sym(bool_val, int_val, doub_val));
    summands.push_back(B);
    summands.push_back(C);

    summation st = sympp::summation(summands);
    // REQUIRE(sum.evaluate_sym(bool_val,int_val,doub_val) == st);

    REQUIRE(sum.evaluate_sym(bool_val, int_val, doub_val).simplify() == 13.3);
}

/*
TEST_CASE("Product") {

    std::vector<double> doub_val(1, 1.0);
    std::vector<int> int_val(2, 1);
    std::vector<uint8_t> bool_val(1, false);

    using namespace sympp;
    sym A(integer(90));
    sym B(real(18.3));
    sym C(integer(-7));
    sym x = sym(variable(var_integer, "x"));
    sym x2 = sym(variable(var_integer, "x2"));

    sym cosA = sym(sympp::cos(A));
    sym absC = sym(sympp::abs(C));

    sym prod = A * absC;

    REQUIRE(prod.is_product());
    REQUIRE(prod.evaluate(bool_val, int_val, doub_val) == 630);

    prod = x * x2 * B * C;
    prod.put_indexes();
    REQUIRE(prod.evaluate(bool_val, int_val, doub_val) == -128.1);
}
*/