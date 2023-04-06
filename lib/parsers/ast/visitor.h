#pragma once

#include <parsers/ast/common.h>

template<typename Node>
void replace_eg(std::unique_ptr<Node>& n) {
    if (n->template is_type<And>() && n->children.back()->template is_type<EG>()) {
        auto fg = make_node<Node, FwdGlobal>();
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

template<typename Node>
void replace_eu(std::unique_ptr<Node>& n) {
    if (n->template is_type<And>() && n->children.back()->template is_type<EU>()) {
        auto fu = make_node<Node, FwdUntil>();
        auto and0 = make_node<Node, And>();
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

template<typename Node>
void replace_ex(std::unique_ptr<Node>& n) {
    if (n->template is_type<And>() && n->children.back()->template is_type<EX>()) {
        auto img = make_node<Node, Img>();
        auto and0 = make_node<Node, And>();
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

template<typename Node>
void replace_root(std::unique_ptr<Node>& n) {
    std::unique_ptr<Node> eq_bool;
    auto p = make_node<Node, p0>();
    auto eq = make_node<Node, EQ>();
    auto and0 = make_node<Node, And>();
    n = std::move(n->children.front());
    if (n->template is_type<Not>()) {
        eq_bool = make_node<Node, False>();
        n = std::move(n->children.front());
    } else {
        eq_bool = make_node<Node, True>();
    }
    add_child(and0, p);
    add_child(and0, n);
    add_child(eq, and0);
    add_child(eq, eq_bool);
    n = std::move(eq);
}

template<typename Node>
void visit(std::unique_ptr<Node>& n) {
    if (n->is_root()) {
        replace_root(n);
    } else if (n->template is_type<And>()) {
        if (n->children.back()->template is_type<EG>()) {
            replace_eg(n);
        } else if (n->children.back()->template is_type<EU>()) {
            replace_eu(n);
            visit(n);
            return;
        } else if (n->children.back()->template is_type<EX>()) {
            replace_ex(n);
            visit(n);
            return;
        }
    }
    for (auto& child : n->children) {
        visit(child);
    }
}
