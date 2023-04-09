#pragma once

#include <parsers/ast/ast.h>

template<typename T>
std::unique_ptr<node_t> make_node() {
    auto node = std::make_unique<node_t>();
    node->set_type<T>();
    return node;
}

int type_prior(const std::unique_ptr<node_t>& n);
void add_child(std::unique_ptr<node_t>& n, std::unique_ptr<node_t>& child);
