#pragma once

#include <parsers/ast/ast.h>

template <typename T>
node_ptr make_node() {
    auto node = std::make_unique<node_t>();
    node->set_type<T>();
    return node;
}

int type_prior(const node_ptr& n);
void add_child(node_ptr& n, node_ptr& child);
