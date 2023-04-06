#include <parsers/cli/cli.h>
#include <parsers/ast/ast.h>
#include <parsers/fsm/fsm.h>
#include <checker/checker.h>

int main(int argc, char *argv[]) {
    const CLI cli(argc, argv);
    auto ast = AST(cli.formula);
    auto fsm = FSM(cli.model);
    check(ast, fsm);
    return 0;
}
