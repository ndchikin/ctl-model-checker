#include <parsers/ast/ast.h>
#include <parsers/ast/rewrite.h>

std::unique_ptr<node_t> make_ast(const std::string& path) {
    file_input input(path);
    auto ast_root = parse_tree::parse<Grammar, Selector>(input);
    visit(ast_root);
    return ast_root;
}
