add_executable(core_tests test_main.cpp)
target_link_libraries(core_tests PRIVATE js_core duktape Catch2::Catch2WithMain)
add_test(NAME core_tests COMMAND core_tests)