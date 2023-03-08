#include <args/args.h>
#include <CTLparser/parser.h>

int main(int argc, char *argv[]) {
    const Args args(argc, argv);
    parseFormula(args.formula);
    return 0;
}
