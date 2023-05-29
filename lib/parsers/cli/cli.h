#pragma once

#include <optional>
#include <string>

struct CLI {
    std::string formula;
    std::optional<std::string> model;
    bool use_bst;
    std::optional<std::string> bdd_stats;
    std::optional<std::string> ast_dot;

    CLI(int argc, const char* const argv[]);
};
