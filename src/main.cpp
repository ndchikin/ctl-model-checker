#include <argparse/argparse.hpp>
#include <config/config.h>
#include <iostream>

int main(int argc, char *argv[]) {
    argparse::ArgumentParser parser("CTL Model Checker", CMAKE_PROJECT_VERSION);

    parser
        .add_argument("-c", "--ctl-formula")
        .help("file path to file with CTL formula")
        .required();

    parser
        .add_argument("-f", "--fsm-model")
        .help("file path to file with FSM model")
        .required();

    try {
        parser.parse_args(argc, argv);
    } catch (const std::runtime_error& err) {
        std::cerr << err.what() << std::endl;
        std::cerr << parser;
        std::exit(1);
    }

    std::cout << parser.get("-c") << std::endl;
    std::cout << parser.get("-f") << std::endl;

    return 0;
}
