#include <parsers/ast/rewrite.h>

std::unique_ptr<node_t> copy(const std::unique_ptr<node_t>& n) {
    auto node = std::make_unique<node_t>();
    node->type = n->type;
    node->m_begin = n->m_begin;
    node->m_end = n->m_end;
    return node;
}

std::unique_ptr<node_t> deep_copy(const std::unique_ptr<node_t>& n) {
    auto node = copy(n);
    for (const auto& child : n->children) {
        node->children.push_back(deep_copy(child));
    }
    return node;
}

void fold(std::unique_ptr<node_t>& n) {
    n = std::move(n->children.front());
}

void replace_not(std::unique_ptr<node_t>& n) {
    auto o = std::move(n->children.front());
    add_child(o, n->children.back());
    n = std::move(o);
}

void replace_or(std::unique_ptr<node_t>& n) {
    auto and0 = make_node<And>();
    auto not0 = make_node<Not>();
    auto not1 = make_node<Not>();
    auto not2 = make_node<Not>();
    auto x = std::move(n->children.front());
    auto y = std::move(n->children.back());
    add_child(not2, y);
    add_child(not1, x);
    add_child(and0, not1);
    add_child(and0, not2);
    add_child(not0, and0);
    n = std::move(not0);
}

void replace_imply(std::unique_ptr<node_t>& n) {
    auto not0 = make_node<Not>();
    auto not1 = make_node<Not>();
    auto and0 = make_node<And>();
    auto x = std::move(n->children.front());
    auto y = std::move(n->children.back());
    add_child(not1, y);
    add_child(and0, x);
    add_child(and0, not1);
    add_child(not0, and0);
    n = std::move(not0);
}

void replace_iff(std::unique_ptr<node_t>& n) {
    auto and0 = make_node<And>();
    auto and1 = make_node<And>();
    auto and2 = make_node<And>();
    auto not0 = make_node<Not>();
    auto not1 = make_node<Not>();
    auto not2 = make_node<Not>();
    auto not3 = make_node<Not>();
    auto x0 = std::move(n->children.front());
    auto x1 = deep_copy(x0);
    auto y0 = std::move(n->children.back());
    auto y1 = deep_copy(y0);
    add_child(not3, x1);
    add_child(and2, not3);
    add_child(and2, y1);
    add_child(not2, y0);
    add_child(and1, x0);
    add_child(and1, not2);
    add_child(not1, and2);
    add_child(not0, and1);
    add_child(and0, not0);
    add_child(and0, not1);
    n = std::move(and0);
}

void Rewrite::transform(std::unique_ptr<node_t>& n) {
    if (n->children.size() == 1) {
        fold(n);
    } else if (n->children.size() == 2) {
        auto o = std::move(n->children.front());
        auto f = std::move(n->children.back());
        add_child(o, f);
        n = std::move(o);
    } else {
        auto f = std::move(n->children.back());
        n->children.pop_back();
        auto o = std::move(n->children.back());
        n->children.pop_back();
        transform(n);
        add_child(o, n);
        add_child(o, f);
        n = std::move(o);
        if (n->is_type<Or>()) {
            replace_or(n);
        } else if (n->is_type<Imply>()) {
            replace_imply(n);
        } else if (n->is_type<IFF>()) {
            replace_iff(n);
        }
    }
}

void Selector<AX>::transform(std::unique_ptr<node_t>& n) {
    auto not0 = make_node<Not>();
    auto not1 = make_node<Not>();
    auto ex = make_node<EX>();
    auto f = std::move(n->children.front());
    add_child(not1, f);
    add_child(ex, not1);
    add_child(not0, ex);
    n = std::move(not0);
}

void Selector<AF>::transform(std::unique_ptr<node_t>& n) {
    auto not0 = make_node<Not>();
    auto not1 = make_node<Not>();
    auto eg = make_node<EG>();
    auto f = std::move(n->children.front());
    add_child(not1, f);
    add_child(eg, not1);
    add_child(not0, eg);
    n = std::move(not0);
}

void Selector<EF>::transform(std::unique_ptr<node_t>& n) {
    auto true0 = make_node<True>();
    auto eu = make_node<EU>();
    auto f = std::move(n->children.front());
    add_child(eu, true0);
    add_child(eu, f);
    n = std::move(eu);
}

void Selector<AG>::transform(std::unique_ptr<node_t>& n) {
    auto true0 = make_node<True>();
    auto not0 = make_node<Not>();
    auto not1 = make_node<Not>();
    auto eu = make_node<EU>();
    auto f = std::move(n->children.front());
    add_child(not1, f);
    add_child(eu, true0);
    add_child(eu, not1);
    add_child(not0, eu);
    n = std::move(not0);
}

void Selector<AU>::transform(std::unique_ptr<node_t>& n) {
    auto or0 = make_node<Or>();
    auto and0 = make_node<And>();
    auto not0 = make_node<Not>();
    auto not1 = make_node<Not>();
    auto not2 = make_node<Not>();
    auto not3 = make_node<Not>();
    auto not4 = make_node<Not>();
    auto eu = make_node<EU>();
    auto eg = make_node<EG>();
    auto g = std::move(n->children.front());
    auto f0 = std::move(n->children.back());
    auto f1 = deep_copy(f0);
    auto f2 = deep_copy(f0);
    add_child(not4, f2);
    add_child(eg, not4);
    add_child(not3, g);
    add_child(not2, f1);
    add_child(and0, not3);
    add_child(and0, not2);
    add_child(not1, f0);
    add_child(eu, not1);
    add_child(eu, and0);
    add_child(or0, eu);
    add_child(or0, eg);
    add_child(not0, or0);
    n = std::move(not0);
}
