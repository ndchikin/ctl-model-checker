#include <config/config.h>
#include <parsers/cli/cli.h>

#include <argparse/argparse.hpp>

CLI::CLI(int argc, const char* const argv[]) {
    argparse::ArgumentParser parser("CTL Model Checker", CMAKE_PROJECT_VERSION);

    parser
        .add_argument("-c", "--ctl-formula")
        .help("file path to file with CTL formula")
        .required();

    parser
        .add_argument("-f", "--fsm-model")
        .help("file path to file with FSM model")
        .required();

    parser
        .add_argument("-o", "--output-states")
        .help("file path to output file")
        .required();

    try {
        parser.parse_args(argc, argv);
    } catch (const std::runtime_error& err) {
        std::cerr << err.what() << std::endl;
        std::cerr << parser;
        std::exit(1);
    }

    formula = parser.get("-c");
    model = parser.get("-f");
    output = parser.get("-o");
}
