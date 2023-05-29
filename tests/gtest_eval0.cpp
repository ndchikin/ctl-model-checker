#include <checker/checker.h>
#include <gtest/gtest.h>
#include <parsers/ast/ast.h>
#include <parsers/fsm/fsm.h>

const char* model0 =
    "3\n"
    "S0 2 p q\n"
    "S1 1 q\n"
    "S2 1 p\n"
    "4\n"
    "S0 S1\n"
    "S1 S0\n"
    "S1 S2\n"
    "S2 S2\n";

struct Model0 : public testing::TestWithParam<bool> {
    static FSM fsm;
};

FSM Model0::fsm(model0);

TEST_P(Model0, Test0) {
    AST ast("true", GetParam());
    EXPECT_TRUE(evaluate_formula(ast, fsm));
}

TEST_P(Model0, Test1) {
    AST ast("false", GetParam());
    EXPECT_FALSE(evaluate_formula(ast, fsm));
}

TEST_P(Model0, Test2) {
    AST ast("p", GetParam());
    EXPECT_TRUE(evaluate_formula(ast, fsm));
}

TEST_P(Model0, Test3) {
    AST ast("q", GetParam());
    EXPECT_TRUE(evaluate_formula(ast, fsm));
}

TEST_P(Model0, Test4) {
    AST ast("f", GetParam());
    EXPECT_FALSE(evaluate_formula(ast, fsm));
}

TEST_P(Model0, Test5) {
    AST ast("¬f", GetParam());
    EXPECT_TRUE(evaluate_formula(ast, fsm));
}

TEST_P(Model0, Test6) {
    AST ast("EX(p)", GetParam());
    EXPECT_FALSE(evaluate_formula(ast, fsm));
}

TEST_P(Model0, Test7) {
    AST ast("EX(q)", GetParam());
    EXPECT_TRUE(evaluate_formula(ast, fsm));
}

TEST_P(Model0, Test8) {
    AST ast("EF(EG(p))", GetParam());
    EXPECT_TRUE(evaluate_formula(ast, fsm));
}

TEST_P(Model0, Test9) {
    AST ast("EG(q)", GetParam());
    EXPECT_TRUE(evaluate_formula(ast, fsm));
}

TEST_P(Model0, Test10) {
    AST ast("AG(p ∨ q)", GetParam());
    EXPECT_TRUE(evaluate_formula(ast, fsm));
}

TEST_P(Model0, Test11) {
    AST ast("AG(q)", GetParam());
    EXPECT_FALSE(evaluate_formula(ast, fsm));
}

TEST_P(Model0, Test12) {
    AST ast("EG(q)", GetParam());
    EXPECT_TRUE(evaluate_formula(ast, fsm));
}

TEST_P(Model0, Test13) {
    AST ast("AX(p ⇔ p)", GetParam());
    EXPECT_TRUE(evaluate_formula(ast, fsm));
}

INSTANTIATE_TEST_SUITE_P(Eval, Model0, testing::Values(true, false));
