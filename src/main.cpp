#include <args/args.h>
#include <iostream>

int main(int argc, char *argv[]) {
    Args args(argc, argv);

    std::cout << args.formula << std::endl;
    std::cout << args.model << std::endl;
    std::cout << args.output << std::endl;

    return 0;
}
