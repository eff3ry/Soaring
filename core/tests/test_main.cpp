#define CATCH_CONFIG_MAIN
#include <catch2/catch_test_macros.hpp>
#include "core.h"

TEST_CASE("JS evaluation works", "[js]") {
    Core core;
    REQUIRE(core.eval_js("2 + 2") == "4");
    REQUIRE(core.eval_js("'hello' + ' world'") == "hello world");
}

TEST_CASE("Native log is available", "[host_api]") {
    Core core;
    REQUIRE(core.eval_js("typeof log") == "function");
}