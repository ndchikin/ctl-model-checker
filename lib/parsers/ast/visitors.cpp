#include <parsers/ast/common.h>
#include <parsers/ast/grammar.h>
#include <parsers/ast/visitors.h>

void replace_eg(std::unique_ptr<node_t>& n) {
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

void replace_eu(std::unique_ptr<node_t>& n) {
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

void replace_ex(std::unique_ptr<node_t>& n) {
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

void replace_root(std::unique_ptr<node_t>& n) {
    std::unique_ptr<node_t> eq_bool;
    auto p = make_node<p0>();
    auto eq = make_node<Eq>();
    auto and0 = make_node<And>();
    n = std::move(n->children.front());
    if (n->is_type<Not>()) {
        eq_bool = make_node<False>();
        n = std::move(n->children.front());
    } else {
        eq_bool = make_node<True>();
    }
    add_child(and0, p);
    add_child(and0, n);
    add_child(eq, and0);
    add_child(eq, eq_bool);
    n = std::move(eq);
}

void rewrite_top(std::unique_ptr<node_t>& n) {
    if (n->is_root()) {
        replace_root(n);
    } else if (n->is_type<And>()) {
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
