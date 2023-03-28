#include <parsers/ctl/ctl.h>
#include <parsers/ctl/rewrite.h>

CTL::CTL(const std::string& path) {
    file_input input(path);
    root = parse_tree::parse<Grammar, Selector>(input);
    visit(root);
}
