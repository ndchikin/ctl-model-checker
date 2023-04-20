#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <parsers/cli/cli.h>

#include <string>

using namespace std::literals::string_literals;

TEST(CLI, ShortArgNames) {
    const char* const argv[] = {"", "-c", "formula.txt", "-f", "model.txt", "-o", "output.txt"};
    const int argc = sizeof(argv) / sizeof(const char*);
    const CLI cli(argc, argv);
    EXPECT_EQ(cli.formula, "formula.txt"s);
    EXPECT_EQ(cli.model, "model.txt"s);
    EXPECT_EQ(cli.output, "output.txt"s);
}

TEST(CLI, FullArgNames) {
    const char* const argv[] = {"", "--ctl-formula", "formula.txt", "--fsm-model", "model.txt", "--output-states", "output.txt"};
    const int argc = sizeof(argv) / sizeof(const char*);
    const CLI cli(argc, argv);
    EXPECT_EQ(cli.formula, "formula.txt"s);
    EXPECT_EQ(cli.model, "model.txt"s);
    EXPECT_EQ(cli.output, "output.txt"s);
}

TEST(CLI, MissingArgName) {
    const char* const argv[] = {"", "-c", "formula.txt", "-o", "output.txt"};
    const int argc = sizeof(argv) / sizeof(const char*);
    EXPECT_EXIT(CLI(argc, argv), testing::ExitedWithCode(1), testing::_);
}
