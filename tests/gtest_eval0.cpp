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

TEST(Eval0, Test0) {
    AST ast("true");
    FSM fsm(model0);
    EXPECT_TRUE(evaluate_formula(ast, fsm));
}

TEST(Eval0, Test1) {
    AST ast("false");
    FSM fsm(model0);
    EXPECT_FALSE(evaluate_formula(ast, fsm));
}

TEST(Eval0, Test2) {
    AST ast("p");
    FSM fsm(model0);
    EXPECT_TRUE(evaluate_formula(ast, fsm));
}

TEST(Eval0, Test3) {
    AST ast("q");
    FSM fsm(model0);
    EXPECT_TRUE(evaluate_formula(ast, fsm));
}

TEST(Eval0, Test4) {
    AST ast("f");
    FSM fsm(model0);
    EXPECT_FALSE(evaluate_formula(ast, fsm));
}

TEST(Eval0, Test5) {
    AST ast("¬f");
    FSM fsm(model0);
    EXPECT_TRUE(evaluate_formula(ast, fsm));
}

TEST(Eval0, Test6) {
    AST ast("EX(p)");
    FSM fsm(model0);
    EXPECT_FALSE(evaluate_formula(ast, fsm));
}

TEST(Eval0, Test7) {
    AST ast("EX(q)");
    FSM fsm(model0);
    EXPECT_TRUE(evaluate_formula(ast, fsm));
}

TEST(Eval0, Test8) {
    AST ast("EF(EG(p))");
    FSM fsm(model0);
    EXPECT_TRUE(evaluate_formula(ast, fsm));
}
