include(GoogleTest)

add_executable(tests gtest_cli.cpp gtest_ast.cpp gtest_checker.cpp)
target_link_libraries(tests PRIVATE GTest::gtest_main GTest::gmock cli ast fsm checker)

gtest_discover_tests(tests)
