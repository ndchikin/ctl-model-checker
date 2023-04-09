#include <parsers/ast/grammar.h>
#include <parsers/ast/rewrite.h>
#include <parsers/ast/visitors.h>

AST::AST(const std::string& path) {
    file_input input(path);
    root = parse_tree::parse<Grammar, node_t, Selector>(input);
    rewrite_top(root);
}
