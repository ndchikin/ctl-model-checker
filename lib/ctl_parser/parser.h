#pragma once

#include <tao/pegtl/contrib/parse_tree.hpp>

std::unique_ptr<tao::pegtl::parse_tree::node> parse_formula(const std::string& path);
