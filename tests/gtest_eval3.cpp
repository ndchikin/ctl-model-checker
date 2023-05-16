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

TEST(Eval3, Test0) {
    AST ast("close ∧ off");
    FSM fsm(model3);
    EXPECT_TRUE(evaluate_formula(ast, fsm));
}

TEST(Eval3, Test1) {
    AST ast("EX(close ∧ on)");
    FSM fsm(model3);
    EXPECT_TRUE(evaluate_formula(ast, fsm));
}

TEST(Eval3, Test2) {
    AST ast("EX(open ∧ off)");
    FSM fsm(model3);
    EXPECT_TRUE(evaluate_formula(ast, fsm));
}

TEST(Eval3, Test3) {
    AST ast("AG(close ∨ off)");
    FSM fsm(model3);
    EXPECT_TRUE(evaluate_formula(ast, fsm));
}}

TEST(Eval3, Test4) {
    AST ast("EF(open ∧ on)");
    FSM fsm(model3);
    EXPECT_FALSE(evaluate_formula(ast, fsm));
}
