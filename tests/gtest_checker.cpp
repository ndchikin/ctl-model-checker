#include <checker/checker.h>
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <parsers/ast/ast.h>
#include <parsers/fsm/fsm.h>

const char* model = "3\n"
                    "S1 2 p q\n"
                    "S2 1 q\n"
                    "S3 1 p\n"
                    "4\n"
                    "S1 S2\n"
                    "S2 S1\n"
                    "S2 S3\n"
                    "S3 S3\n";

TEST(FSM, Test0) {
    AST ast("p");
    FSM fsm(model);
    EXPECT_TRUE(evaluate_formula(ast, fsm));
}

TEST(FSM, Test1) {
    AST ast("q");
    FSM fsm(model);
    EXPECT_TRUE(evaluate_formula(ast, fsm));
}

TEST(FSM, Test2) {
    AST ast("f");
    FSM fsm(model);
    EXPECT_FALSE(evaluate_formula(ast, fsm));
}
