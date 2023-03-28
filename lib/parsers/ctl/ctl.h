#pragma once

#include <tao/pegtl/contrib/parse_tree.hpp>

class CTL {
private:
    std::unique_ptr<tao::pegtl::parse_tree::node> root;

public:
    CTL(const std::string& path);
};
