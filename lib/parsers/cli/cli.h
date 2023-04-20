#pragma once

#include <string>

struct CLI {
    std::string formula;
    std::string model;

    CLI(int argc, const char* const argv[]);
};
