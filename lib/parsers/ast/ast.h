#pragma once

#include <tao/pegtl/contrib/parse_tree.hpp>

class AST {
private:
    std::unique_ptr<tao::pegtl::parse_tree::node> root;

public:
    AST(const std::string& path);
};
