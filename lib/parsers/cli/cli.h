#pragma once

#include <string>

struct CLI {
    std::string formula;
    std::string model;
    std::string output;

    CLI(int argc, const char* const argv[]);
};
