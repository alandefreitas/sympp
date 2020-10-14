#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>
#include <sympp/sympp.h>
#include <sympp/node/terminal/rational.h>

TEST_CASE("Numbers") {

    std::vector<double> doub_val(1, 1.0);
    std::vector<int> int_val(1, 1);
    std::vector<uint8_t> bool_val(1, false);

    using namespace sympp;
    sym n(integer(10.3));
    REQUIRE(n == 10);
    REQUIRE(n > 9);
    REQUIRE(n < 11);

    REQUIRE(n.evaluate(bool_val,int_val,doub_val) == 10);

    REQUIRE(n.evaluate_sym(bool_val,int_val,doub_val) == sym(integer(10)));

    REQUIRE(n.evaluate_sym(bool_val,int_val,doub_val).simplify() == 10);

    n += 1;
    REQUIRE(n.is_summation());

}


TEST_CASE("Real Number"){

    std::vector<double> doub_val(1, 1.0);
    std::vector<int> int_val(1, 1);
    std::vector<uint8_t> bool_val(1, false);

    using namespace sympp;
    sym n(real(10.5));
    REQUIRE(n == 10.5);
    REQUIRE(n > 9);
    REQUIRE(n < 11);

    REQUIRE(n.evaluate(bool_val,int_val,doub_val) == 10.5);

    REQUIRE(n.evaluate_sym(bool_val,int_val,doub_val) == sym(real(10.5)));

    REQUIRE(n.evaluate_sym(bool_val,int_val,doub_val).simplify() == 10.5);

    n += 1;
    REQUIRE(n.is_summation());

}

TEST_CASE("Booleans"){


    std::vector<double> doub_val(1, 1.0);
    std::vector<int> int_val(1, 1);
    std::vector<uint8_t> bool_val(1, false);

    using namespace sympp;
    sym x(true);
    sym y(false);
    REQUIRE(x == true);
    REQUIRE(x != false);
    REQUIRE(x != y);

    REQUIRE(x.evaluate(bool_val,int_val,doub_val) == 1);

    REQUIRE(x.evaluate_sym(bool_val,int_val,doub_val) == sym(true));

    REQUIRE(x.evaluate_sym(bool_val,int_val,doub_val).simplify() == 1);

    x += 1;
    REQUIRE(x.is_summation());

}


TEST_CASE("Constants"){

    std::vector<double> doub_val(1, 1.0);
    std::vector<int> int_val(1, 1);
    std::vector<uint8_t> bool_val(1, false);

    using namespace sympp;
    sym e = constant::e();
    sym pi = constant::pi();
    sym e1 = sym(constant("e", 2.71828));
    sym pi1 = sym(constant("pi1", 3.14));

    REQUIRE(e != pi);
    REQUIRE(e == e1);
    REQUIRE(pi != pi1);
    REQUIRE(e.evaluate(bool_val,int_val,doub_val) == 2.71828);
    REQUIRE(e.evaluate(bool_val,int_val,doub_val) < pi.evaluate(bool_val,int_val,doub_val));

    REQUIRE(e.evaluate_sym(bool_val,int_val,doub_val) == sym(real(2.71828)));

    REQUIRE(e.evaluate_sym(bool_val,int_val,doub_val).simplify() == 2.71828);


}


TEST_CASE("Rational"){

    std::vector<double> doub_val(1, 1.0);
    std::vector<int> int_val(1, 1);
    std::vector<uint8_t> bool_val(1, false);

    using namespace sympp;
    sym ratio = sym(rational(100, 100));
    sym ratio2 = sym(rational(15, 15));
    sym ratio3 = sym(rational(15, 15));

    REQUIRE(ratio == ratio3);
    REQUIRE(ratio == ratio2);

    REQUIRE(ratio.evaluate(bool_val,int_val,doub_val) == 1);

    REQUIRE(ratio.evaluate_sym(bool_val,int_val,doub_val) == sym(integer(1)));

    REQUIRE(ratio.evaluate_sym(bool_val,int_val,doub_val).simplify() == 1);

}


TEST_CASE("Variables"){

    std::vector<double> doub_val(1, 1.0);
    std::vector<int> int_val(2, 2);
    std::vector<uint8_t> bool_val(1, true);

    using namespace sympp;
    sym x = sym(variable(var_integer, "x"));
    sym x2 = sym(variable(var_integer, "x2"));
    sym y = sym(variable(var_boolean, "y"));
    sym z = sym(variable(var_integer, "z"));


    REQUIRE(x != y);
    REQUIRE(x != z);

    x.put_indexes();
    REQUIRE(x.evaluate(bool_val,int_val,doub_val) == 2);

    REQUIRE(x.evaluate_sym(bool_val,int_val,doub_val) == sym(integer(2)));

    REQUIRE(x.evaluate_sym(bool_val,int_val,doub_val).simplify() == 2);

    sym sum = x + x2 + y;
    sum.put_indexes();
    
    REQUIRE(sum.evaluate(bool_val,int_val,doub_val) == 5);

    REQUIRE(sum.evaluate_sym(bool_val,int_val,doub_val) == sym(integer(0)) + sym(integer(2)) + sym(integer(2)) + sym(true));

    REQUIRE(sum.evaluate_sym(bool_val,int_val,doub_val).simplify() == 5);


}


