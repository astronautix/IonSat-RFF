#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include <iostream>
#include "FFS.h"

TEST_CASE("Creation and destruction", "[Event]") {
    std::cout << "ok" << std::endl;
    REQUIRE(1 == 1);
}
