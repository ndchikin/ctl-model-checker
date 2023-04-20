#pragma once

#include <memory>
#include <tao/pegtl.hpp>
#include <tao/pegtl/contrib/parse_tree.hpp>

using node_t = tao::pegtl::parse_tree::node;
using node_ptr = std::unique_ptr<node_t>;
using file_input_ptr = std::unique_ptr<tao::pegtl::file_input<>>;
using memory_input_ptr = std::unique_ptr<tao::pegtl::memory_input<>>;

class AST {
private:
    void build_ast();

public:
    node_ptr root;
    file_input_ptr file;
    memory_input_ptr memory;

    explicit AST(const std::string& path);
    explicit AST(const char* str);
};
