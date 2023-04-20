#include <checker/checker.h>
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <parsers/ast/ast.h>
#include <parsers/fsm/fsm.h>

const char* model2 =
    "8\n"
    "S0 4 end1 end2 idle1 idle2\n"
    "S1 3 req1 end2 idle2\n"
    "S2 3 req2 end1 idle1\n"
    "S3 2 req2 req1\n"
    "S4 2 cs1 idle2\n"
    "S5 2 cs2 idle1\n"
    "S6 2 cs1 req2\n"
    "S7 2 cs2 req1\n"
    "14\n"
    "S0 S1\n"
    "S0 S2\n"
    "S1 S3\n"
    "S1 S4\n"
    "S2 S3\n"
    "S2 S5\n"
    "S3 S6\n"
    "S3 S7\n"
    "S4 S0\n"
    "S4 S6\n"
    "S5 S0\n"
    "S5 S7\n"
    "S6 S2\n"
    "S7 S1\n";

TEST(Eval2, Test0) {
    AST ast("req1");
    FSM fsm(model2);
    EXPECT_FALSE(evaluate_formula(ast, fsm));
}

TEST(Eval2, Test1) {
    AST ast("¬req1");
    FSM fsm(model2);
    EXPECT_TRUE(evaluate_formula(ast, fsm));
}

TEST(Eval2, Test2) {
    AST ast("end1");
    FSM fsm(model2);
    EXPECT_TRUE(evaluate_formula(ast, fsm));
}

TEST(Eval2, Test3) {
    AST ast("EX(req1)");
    FSM fsm(model2);
    EXPECT_TRUE(evaluate_formula(ast, fsm));
}

TEST(Eval2, Test4) {
    AST ast("EG(idle1)");
    FSM fsm(model2);
    EXPECT_TRUE(evaluate_formula(ast, fsm));
}

TEST(Eval2, Test5) {
    AST ast("A[idle1 U req1]");
    FSM fsm(model2);
    EXPECT_FALSE(evaluate_formula(ast, fsm));
}

TEST(Eval2, Test6) {
    AST ast("E[idle1 U req1]");
    FSM fsm(model2);
    EXPECT_TRUE(evaluate_formula(ast, fsm));
}

TEST(Eval2, Test7) {
    AST ast("EX(EG(req2))");
    FSM fsm(model2);
    EXPECT_TRUE(evaluate_formula(ast, fsm));
}

TEST(Eval2, Test8) {
    AST ast("A[end1 U req1 ∨ idle1]");
    FSM fsm(model2);
    EXPECT_TRUE(evaluate_formula(ast, fsm));
}

TEST(Eval2, Test9) {
    AST ast("A[end2 U req2 ∨ idle2]");
    FSM fsm(model2);
    EXPECT_TRUE(evaluate_formula(ast, fsm));
}
