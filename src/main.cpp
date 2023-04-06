#include <parsers/cli/cli.h>
#include <parsers/ast/ast.h>
#include <parsers/fsm/fsm.h>
#include <checker/checker.h>

int main(int argc, char *argv[]) {
    const CLI cli(argc, argv);
    const auto ast_root = make_ast(cli.formula);
    auto fsm = FSM(cli.model);
    auto states = evaluate_formula(ast_root, fsm);
    write_states(cli.output, states);
    return 0;
}
