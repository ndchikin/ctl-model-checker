#include <checker/checker.h>
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <parsers/ast/ast.h>
#include <parsers/fsm/fsm.h>

const char* model1 =
    "4\n"
    "S0 2 a b\n"
    "S1 1 b\n"
    "S2 1 b\n"
    "S3 1 c\n"
    "7\n"
    "S0 S1\n"
    "S0 S2\n"
    "S1 S1\n"
    "S1 S2\n"
    "S2 S0\n"
    "S2 S3\n"
    "S3 S1\n";

TEST(Eval1, Test0) {
    AST ast("AG(EF(a))");
    FSM fsm(model1);
    EXPECT_TRUE(evaluate_formula(ast, fsm));
}

TEST(Eval1, Test1) {
    AST ast("A[b U c]");
    FSM fsm(model1);
    EXPECT_FALSE(evaluate_formula(ast, fsm));
}
