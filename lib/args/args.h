#include <string>

struct Args {
    std::string formula;
    std::string model;
    std::string output;

    Args(int argc, char *argv[]);
};
