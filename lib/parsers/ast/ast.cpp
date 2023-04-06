#include <parsers/ast/ast.h>
#include <parsers/ast/rewrite.h>

AST::AST(const std::string& path) {
    file_input input(path);
    root = parse_tree::parse<Grammar, Selector>(input);
    visit(root);
}
