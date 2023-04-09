#pragma once

#include <tao/pegtl/contrib/parse_tree.hpp>

typedef tao::pegtl::parse_tree::node node_t;

struct AST {
    std::unique_ptr<node_t> root;

    AST(const std::string& path);
};
