#include <args/args.h>
#include <ctl_parser/parser.h>

int main(int argc, char *argv[]) {
    const Args args(argc, argv);
    auto ast_root = parse_formula(args.formula);
    return 0;
}
