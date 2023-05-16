#include <config/config.h>
#include <parsers/cli/cli.h>

#include <argparse/argparse.hpp>

CLI::CLI(int argc, const char* const argv[]) {
    argparse::ArgumentParser parser("ctl-model-checker", CMAKE_PROJECT_VERSION);

    parser
        .add_argument("-c", "--ctl-formula")
        .help("file path to file with CTL formula")
        .required();

    parser
        .add_argument("-f", "--fsm-model")
        .help("file path to file with FSM model")
        .required();

    parser
        .add_argument("--use-bst")
        .help("use backward state traversal, don't rewrites temporal operators using forward state traversal")
        .default_value(false)
        .implicit_value(true);

    parser
        .add_argument("--bdd-stats")
        .help("file path to file where to dump BDD usage statistic");

    parser
        .add_argument("--ast-dot")
        .help("file path to file where to dump evaluated AST");

    try {
        parser.parse_args(argc, argv);
    } catch (const std::runtime_error& err) {
        std::cerr << err.what() << std::endl;
        std::cerr << parser;
        std::exit(1);
    }

    formula = parser.get("-c");
    model = parser.get("-f");
    use_bst = parser.get<bool>("--use-bst");
    bdd_stats = parser.present("--bdd-stats");
    ast_dot = parser.present("--ast-dot");
}
