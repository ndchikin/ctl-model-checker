#include <parsers/ast/common.h>
#include <parsers/ast/grammar.h>

int type_prior(const node_ptr& n) {
    if (n->is_type<EG>()) {
        return 3;
    } else if (n->is_type<EU>()) {
        return 2;
    } else if (n->is_type<EX>()) {
        return 1;
    } else {
        return 0;
    }
}

void add_child(node_ptr& n, node_ptr& child) {
    if (n->is_type<Not>() && child->is_type<Not>()) {
        n = std::move(child->children.front());
    } else if (n->is_type<And>() && child->is_type<And>()) {
        for (auto& c : child->children) {
            add_child(n, c);
        }
        std::sort(
            n->children.begin(),
            n->children.end(),
            [](const node_ptr& n0, const node_ptr& n1) {
                return type_prior(n0) < type_prior(n1);
            }
        );
    } else {
        n->children.push_back(std::move(child));
    }
}
