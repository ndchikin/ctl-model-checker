#pragma once

#include <parsers/ctl/grammar.h>

template<typename Node, typename T>
std::unique_ptr<Node> make_node() {
    auto node = std::make_unique<Node>();
    node->template set_type<T>();
    return node;
}

template<typename Node>
int type_prior(const std::unique_ptr<Node>& n) {
    if (n->template is_type<EG>()) {
        return 3;
    } else if (n->template is_type<EU>()) {
        return 2;
    } else if (n->template is_type<EX>()) {
        return 1;
    } else {
        return 0;
    }
}

template<typename Node>
void add_child(std::unique_ptr<Node>& n, std::unique_ptr<Node>& child) {
    if (n->template is_type<Not>() && child->template is_type<Not>()) {
        n = std::move(child->children.front());
    } else if (n->template is_type<And>() && child->template is_type<And>()) {
        for (auto& c : child->children) {
            add_child(n, c);
        }
        std::sort(
            n->children.begin(), n->children.end(),
            [](const std::unique_ptr<Node>& n0, const std::unique_ptr<Node>& n1) {
                return type_prior(n0) < type_prior(n1);
            }
        );
    } else {
        n->children.push_back(std::move(child));
    }
}
