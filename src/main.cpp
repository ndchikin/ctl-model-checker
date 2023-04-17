#include <checker/checker.h>
#include <parsers/ast/ast.h>
#include <parsers/cli/cli.h>
#include <parsers/fsm/fsm.h>

int main(int argc, char *argv[]) {
    const CLI cli(argc, argv);
    const AST ast(cli.formula);
    const FSM fsm(cli.model);
    evaluate_formula(ast, fsm, cli.output);
    return 0;
}
