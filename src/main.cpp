#include <checker/checker.h>
#include <parsers/ast/ast.h>
#include <parsers/cli/cli.h>
#include <parsers/fsm/fsm.h>

#include <iostream>

int main(int argc, char* argv[]) {
    const CLI cli(argc, argv);
    const AST ast(cli.formula, cli.use_bst);
    if (cli.model.has_value()) {
        const FSM fsm(cli.model.value());
        std::cout << evaluate_formula(ast, fsm) << std::endl;
        if (cli.bdd_stats.has_value()) {
            sylvan_stats(cli.bdd_stats.value());
        }
    }
    if (cli.ast_dot.has_value()) {
        ast.to_dot(cli.ast_dot.value());
    }
    return 0;
}
