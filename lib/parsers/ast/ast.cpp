#include <parsers/ast/ast.h>
#include <parsers/ast/grammar.h>
#include <parsers/ast/rewrite.h>
#include <parsers/ast/visitors.h>

#include <memory>

void AST::build_ast() {
    if (file) {
        root = parse_tree::parse<Grammar, node_t, Selector>(*file);
    } else {
        root = parse_tree::parse<Grammar, node_t, Selector>(*memory);
    }
    rewrite_root(root);
    rewrite_top(root);
}

AST::AST(const std::string& path) : file(new file_input<>(path)) {
    build_ast();
}

AST::AST(const char* str) : memory(new memory_input<>(str, "")) {
    build_ast();
}
