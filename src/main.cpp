#include <checker/checker.h>
#include <parsers/ast/ast.h>
#include <parsers/cli/cli.h>
#include <parsers/fsm/fsm.h>

#include <iostream>

int main(int argc, char* argv[]) {
    const CLI cli(argc, argv);
    const AST ast(cli.formula, cli.use_bst);
    const FSM fsm(cli.model, cli.one_partition);
    std::cout << evaluate_formula(ast, fsm) << std::endl;
    if (cli.sylvan_stats.has_value()) {
        sylvan_stats(cli.sylvan_stats.value());
    }
    return 0;
}
