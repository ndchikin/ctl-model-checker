#include <checker/checker.h>
#include <parsers/ast/ast.h>
#include <parsers/cli/cli.h>
#include <parsers/fsm/fsm.h>

#include <iostream>

int main(int argc, char *argv[]) {
    const CLI cli(argc, argv);
    const AST ast(cli.formula);
    const FSM fsm(cli.model);
    std::cout << evaluate_formula(ast, fsm);
    return 0;
}
