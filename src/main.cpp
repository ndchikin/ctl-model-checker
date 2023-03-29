#include <parsers/args/args.h>
#include <parsers/ctl/ctl.h>
#include <parsers/fsm/fsm.h>

int main(int argc, char *argv[]) {
    const Args args(argc, argv);
    auto ast = CTL(args.formula);
    auto fsm = FSM(args.model);
    return 0;
}
