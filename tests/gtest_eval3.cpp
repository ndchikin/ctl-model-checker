#include <checker/checker.h>
#include <gtest/gtest.h>
#include <parsers/ast/ast.h>
#include <parsers/fsm/fsm.h>

const char* model3 =
    "3\n"
    "S0 2 close off\n"
    "S1 2 open off\n"
    "S2 2 close on\n"
    "4\n"
    "S0 S1\n"
    "S1 S0\n"
    "S0 S2\n"
    "S2 S0\n";

struct Model3 : public testing::TestWithParam<bool> {
    static FSM fsm;
};

FSM Model3::fsm(model3);

TEST_P(Model3, Test0) {
    AST ast("close ∧ off", GetParam());
    EXPECT_TRUE(evaluate_formula(ast, fsm));
}

TEST_P(Model3, Test1) {
    AST ast("EX(close ∧ on)", GetParam());
    EXPECT_TRUE(evaluate_formula(ast, fsm));
}

TEST_P(Model3, Test2) {
    AST ast("EX(open ∧ off)", GetParam());
    EXPECT_TRUE(evaluate_formula(ast, fsm));
}

TEST_P(Model3, Test3) {
    AST ast("AG(close ∨ off)", GetParam());
    EXPECT_TRUE(evaluate_formula(ast, fsm));
}

TEST_P(Model3, Test4) {
    AST ast("EF(open ∧ on)", GetParam());
    EXPECT_FALSE(evaluate_formula(ast, fsm));
}

INSTANTIATE_TEST_SUITE_P(Eval, Model3, testing::Values(true, false));
