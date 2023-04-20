#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <parsers/cli/cli.h>

#include <string>

using namespace std::literals::string_literals;

TEST(CLI, ShortArgNames) {
    const char* const argv[] = {"", "-c", "formula.txt", "-f", "model.txt"};
    const int argc = sizeof(argv) / sizeof(const char*);
    const CLI cli(argc, argv);
    EXPECT_EQ(cli.formula, "formula.txt"s);
    EXPECT_EQ(cli.model, "model.txt"s);
}

TEST(CLI, FullArgNames) {
    const char* const argv[] = {"", "--ctl-formula", "formula.txt", "--fsm-model", "model.txt"};
    const int argc = sizeof(argv) / sizeof(const char*);
    const CLI cli(argc, argv);
    EXPECT_EQ(cli.formula, "formula.txt"s);
    EXPECT_EQ(cli.model, "model.txt"s);
}

TEST(CLI, MissingArgName) {
    const char* const argv[] = {"", "-c", "formula.txt"};
    const int argc = sizeof(argv) / sizeof(const char*);
    EXPECT_EXIT(CLI(argc, argv), testing::ExitedWithCode(1), testing::_);
}

TEST(CLI, MissingArgValue) {
    const char* const argv[] = {"", "-c", "formula.txt", "-f"};
    const int argc = sizeof(argv) / sizeof(const char*);
    EXPECT_EXIT(CLI(argc, argv), testing::ExitedWithCode(1), testing::_);
}

TEST(CLI, Help) {
    const char* const argv[] = {"", "-h"};
    const int argc = sizeof(argv) / sizeof(const char*);
    EXPECT_EXIT(CLI(argc, argv), testing::ExitedWithCode(0), testing::_);
}

TEST(CLI, Version) {
    const char* const argv[] = {"", "-v"};
    const int argc = sizeof(argv) / sizeof(const char*);
    EXPECT_EXIT(CLI(argc, argv), testing::ExitedWithCode(0), testing::_);
}
