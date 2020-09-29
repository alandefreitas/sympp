#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>
#include <sympp/sympp.h>

TEST_CASE("Numbers") {
    using namespace sympp;
    sym n(integer(10));
    REQUIRE(n == 10);
    REQUIRE(n > 9);
    REQUIRE(n < 11);
    REQUIRE(n == 10);
    REQUIRE(n > 9);
    REQUIRE(n < 11);
    n += 1;
    REQUIRE(n.is_summation());
}