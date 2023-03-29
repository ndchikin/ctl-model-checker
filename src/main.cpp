#include <parsers/cli/cli.h>
#include <parsers/ctl/ctl.h>
#include <parsers/fsm/fsm.h>

int main(int argc, char *argv[]) {
    const CLI cli(argc, argv);
    auto ast = CTL(cli.formula);
    auto fsm = FSM(cli.model);
    fsm.print();
    return 0;
}
