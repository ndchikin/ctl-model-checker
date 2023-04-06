#pragma once

#include <tao/pegtl/contrib/parse_tree.hpp>

typedef tao::pegtl::parse_tree::node node_t;

std::unique_ptr<node_t> make_ast(const std::string& path);
