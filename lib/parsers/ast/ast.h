#pragma once

#include <tao/pegtl/contrib/parse_tree.hpp>

using node_t = tao::pegtl::parse_tree::node;
using node_ptr = std::unique_ptr<node_t>;

struct AST {
    node_ptr root;

    explicit AST(const std::string& path);
};
