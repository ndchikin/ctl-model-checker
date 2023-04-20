include(GoogleTest)

add_executable(tests gtest_cli.cpp gtest_ast.cpp gtest_eval0.cpp gtest_eval1.cpp gtest_eval2.cpp)
target_link_libraries(tests PRIVATE GTest::gtest_main GTest::gmock cli checker)

gtest_discover_tests(tests)
