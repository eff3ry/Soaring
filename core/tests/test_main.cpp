#define CATCH_CONFIG_MAIN
#include <catch2/catch_test_macros.hpp>
#include "core.h"
#include <fstream>

TEST_CASE("JS evaluation works", "[js]") {
    Core core;
    REQUIRE(core.eval_js("2 + 2") == "4");
    REQUIRE(core.eval_js("'hello' + ' world'") == "hello world");
    REQUIRE(core.eval_js("typeof log") == "function");
}

TEST_CASE("File read/write via JS", "[js][file]") {
    Core core;
    std::string testPath = "testfile.txt";

    // Clean up any old file
    std::remove(testPath.c_str());

    // Write file from JS
    REQUIRE(core.eval_js("writeFile('testfile.txt', 'hello from js')") == "OK");

    // Read file from JS
    REQUIRE(core.eval_js("readFile('testfile.txt')") == "hello from js");

    // Clean up
    std::remove(testPath.c_str());
}

TEST_CASE("Fetch HTTP", "[js][fetch]") {
    Core core;
    std::string result = core.eval_js("fetch('https://httpbin.org/get')");
    // Should contain JSON with url key
    REQUIRE(result.find("\"url\":") != std::string::npos);
}