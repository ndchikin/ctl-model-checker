#include <parsers/ast/common.h>
#include <parsers/ast/grammar.h>
#include <parsers/ast/visitors.h>

void replace_eg(node_ptr& n) {
    if (n->is_type<And>() && n->children.back()->is_type<EG>()) {
        auto fg = make_node<FG>();
        auto eg = std::move(n->children.back());
        auto q = std::move(eg->children.front());
        n->children.pop_back();
        if (n->children.size() == 1) {
            n = std::move(n->children.front());
        }
        add_child(fg, n);
        add_child(fg, q);
        n = std::move(fg);
    }
}

void replace_eu(node_ptr& n) {
    if (n->is_type<And>() && n->children.back()->is_type<EU>()) {
        auto fu = make_node<FU>();
        auto and0 = make_node<And>();
        auto eu = std::move(n->children.back());
        auto q = std::move(eu->children.front());
        auto f = std::move(eu->children.back());
        n->children.pop_back();
        if (n->children.size() == 1) {
            n = std::move(n->children.front());
        }
        add_child(fu, n);
        add_child(fu, q);
        add_child(and0, fu);
        add_child(and0, f);
        n = std::move(and0);
    }
}

void replace_ex(node_ptr& n) {
    if (n->is_type<And>() && n->children.back()->is_type<EX>()) {
        auto img = make_node<Img>();
        auto and0 = make_node<And>();
        auto ex = std::move(n->children.back());
        auto f = std::move(ex->children.front());
        n->children.pop_back();
        if (n->children.size() == 1) {
            n = std::move(n->children.front());
        }
        add_child(img, n);
        add_child(and0, img);
        add_child(and0, f);
        n = std::move(and0);
    }
}

void rewrite_root(node_ptr& n) {
    auto eq = make_node<Eq>();
    auto p = make_node<p0>();
    auto and0 = make_node<And>();
    auto false0 = make_node<False>();
    auto f = std::move(n->children.front());
    if (f->is_type<Not>()) {
        f = std::move(f->children.front());
        add_child(and0, p);
        add_child(and0, f);
        add_child(eq, and0);
        add_child(eq, false0);
        n = std::move(eq);
    } else {
        auto not0 = make_node<Not>();
        add_child(and0, p);
        add_child(and0, f);
        add_child(eq, and0);
        add_child(eq, false0);
        add_child(not0, eq);
        n = std::move(not0);
    }
}

void rewrite_top(node_ptr& n) {
    if (n->is_type<And>()) {
        if (n->children.back()->is_type<EG>()) {
            replace_eg(n);
        } else if (n->children.back()->is_type<EU>()) {
            replace_eu(n);
            rewrite_top(n);
            return;
        } else if (n->children.back()->is_type<EX>()) {
            replace_ex(n);
            rewrite_top(n);
            return;
        }
    }
    for (auto& child : n->children) {
        rewrite_top(child);
    }
}
