include(GoogleTest)
include(CodeCoverage)

add_executable(tests gtest_ast.cpp gtest_eval0.cpp gtest_eval1.cpp gtest_eval2.cpp gtest_eval3.cpp)
target_link_libraries(tests PRIVATE GTest::gtest_main GTest::gmock cli checker)

gtest_discover_tests(tests)

setup_target_for_coverage_lcov(
    NAME coverage
    EXECUTABLE tests
    INCLUDE "lib/*"
)
