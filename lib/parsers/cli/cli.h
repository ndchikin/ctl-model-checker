#pragma once

#include <optional>
#include <string>

struct CLI {
    std::string formula;
    std::string model;
    bool use_bst;
    bool one_partition;
    std::optional<std::string> sylvan_stats;

    CLI(int argc, const char* const argv[]);
};
