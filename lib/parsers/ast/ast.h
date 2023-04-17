#pragma once

#include <tao/pegtl/contrib/parse_tree.hpp>

using node_t = tao::pegtl::parse_tree::node;

struct AST {
    std::unique_ptr<node_t> root;

    explicit AST(const std::string& path);
};
