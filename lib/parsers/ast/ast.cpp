#include <parsers/ast/ast.h>
#include <parsers/ast/grammar.h>
#include <parsers/ast/rewrite.h>
#include <parsers/ast/visitors.h>

#include <memory>

void AST::build_ast(bool use_bst) {
    if (file) {
        root = parse_tree::parse<Grammar, node_t, Selector>(*file);
    } else {
        root = parse_tree::parse<Grammar, node_t, Selector>(*memory);
    }
    rewrite_root(root);
    if (!use_bst) {
        rewrite_top(root);
    }
}

AST::AST(const std::string& path, bool use_bst) : file(new file_input<>(path)) {
    build_ast(use_bst);
}

AST::AST(const char* str, bool use_bst) : memory(new memory_input<>(str, "")) {
    build_ast(use_bst);
}
