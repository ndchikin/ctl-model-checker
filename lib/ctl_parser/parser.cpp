#include <ctl_parser/common.h>
#include <ctl_parser/parser.h>
#include <ctl_parser/visitor.h>
#include <iostream>
#include <tao/pegtl/contrib/parse_tree_to_dot.hpp>

template<typename Node>
std::unique_ptr<Node> copy(const std::unique_ptr<Node>& n) {
    auto node = std::make_unique<Node>();
    node->type = n->type;
    node->m_begin = n->m_begin;
    node->m_end = n->m_end;
    return node;
}

template<typename Node>
std::unique_ptr<Node> deep_copy(const std::unique_ptr<Node>& n) {
    auto node = copy(n);
    for (const auto& child : n->children) {
        node->children.push_back(deep_copy(child));
    }
    return node;
}

template<typename Node>
void fold(std::unique_ptr<Node>& n) {
    n = std::move(n->children.front());
}

template<typename Node>
void replace_not(std::unique_ptr<Node>& n) {
    auto o = std::move(n->children.front());
    add_child(o, n->children.back());
    n = std::move(o);
}

template<typename Node>
void replace_or(std::unique_ptr<Node>& n) {
    auto and0 = make_node<Node, And>();
    auto not0 = make_node<Node, Not>();
    auto not1 = make_node<Node, Not>();
    auto not2 = make_node<Node, Not>();
    auto x = std::move(n->children.front());
    auto y = std::move(n->children.back());
    add_child(not2, y);
    add_child(not1, x);
    add_child(and0, not1);
    add_child(and0, not2);
    add_child(not0, and0);
    n = std::move(not0);
}

template<typename Node>
void replace_imply(std::unique_ptr<Node>& n) {
    auto not0 = make_node<Node, Not>();
    auto not1 = make_node<Node, Not>();
    auto and0 = make_node<Node, And>();
    auto x = std::move(n->children.front());
    auto y = std::move(n->children.back());
    add_child(not1, y);
    add_child(and0, x);
    add_child(and0, not1);
    add_child(not0, and0);
    n = std::move(not0);
}

template<typename Node>
void replace_iff(std::unique_ptr<Node>& n) {
    auto and0 = make_node<Node, And>();
    auto and1 = make_node<Node, And>();
    auto and2 = make_node<Node, And>();
    auto not0 = make_node<Node, Not>();
    auto not1 = make_node<Node, Not>();
    auto not2 = make_node<Node, Not>();
    auto not3 = make_node<Node, Not>();
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

struct Rewrite : parse_tree::apply<Rewrite> {
    template<typename Node, typename... States>
    static void transform(std::unique_ptr<Node>& n, States&&... st) {
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
            transform(n, st...);
            add_child(o, n);
            add_child(o, f);
            n = std::move(o);
            if (n->template is_type<Or>()) {
                replace_or(n);
            } else if (n->template is_type<Imply>()) {
                replace_imply(n);
            } else if (n->template is_type<IFF>()) {
                replace_iff(n);
            }
        }
    }
};

template<typename Rule>
struct Selector : parse_tree::selector<
    Rule,
    Rewrite::on<
        P0,
        P1,
        P2,
        P3,
        P4,
        P5
    >,
    parse_tree::store_content::on<
        Atom
    >,
    parse_tree::remove_content::on<
        True,
        False,
        Not,
        And,
        Or,
        Imply,
        IFF,
        EX,
        EG,
        EU
    >
> {};

template<>
struct Selector<AX> : std::true_type {
    template<typename Node, typename... States>
    static void transform(std::unique_ptr<Node>& n, States&&... st) {
        auto not0 = make_node<Node, Not>();
        auto not1 = make_node<Node, Not>();
        auto ex = make_node<Node, EX>();
        auto f = std::move(n->children.front());
        add_child(not1, f);
        add_child(ex, not1);
        add_child(not0, ex);
        n = std::move(not0);
    }
};

template<>
struct Selector<AF> : std::true_type {
    template<typename Node, typename... States>
    static void transform(std::unique_ptr<Node>& n, States&&... st) {
        auto not0 = make_node<Node, Not>();
        auto not1 = make_node<Node, Not>();
        auto eg = make_node<Node, EG>();
        auto f = std::move(n->children.front());
        add_child(not1, f);
        add_child(eg, not1);
        add_child(not0, eg);
        n = std::move(not0);
    }
};

template<>
struct Selector<EF> : std::true_type {
    template<typename Node, typename... States>
    static void transform(std::unique_ptr<Node>& n, States&&... st) {
        auto true0 = make_node<Node, True>();
        auto eu = make_node<Node, EU>();
        auto f = std::move(n->children.front());
        add_child(eu, true0);
        add_child(eu, f);
        n = std::move(eu);
    }
};

template<>
struct Selector<AG> : std::true_type {
    template<typename Node, typename... States>
    static void transform(std::unique_ptr<Node>& n, States&&... st) {
        auto true0 = make_node<Node, True>();
        auto not0 = make_node<Node, Not>();
        auto not1 = make_node<Node, Not>();
        auto eu = make_node<Node, EU>();
        auto f = std::move(n->children.front());
        add_child(not1, f);
        add_child(eu, true0);
        add_child(eu, not1);
        add_child(not0, eu);
        n = std::move(not0);
    }
};

template<>
struct Selector<AU> : std::true_type {
    template<typename Node, typename... States>
    static void transform(std::unique_ptr<Node>& n, States&&... st) {
        auto or0 = make_node<Node, Or>();
        auto and0 = make_node<Node, And>();
        auto not0 = make_node<Node, Not>();
        auto not1 = make_node<Node, Not>();
        auto not2 = make_node<Node, Not>();
        auto not3 = make_node<Node, Not>();
        auto not4 = make_node<Node, Not>();
        auto eu = make_node<Node, EU>();
        auto eg = make_node<Node, EG>();
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
};

std::unique_ptr<parse_tree::node> parse_formula(const std::string& path) {
    file_input input(path);
    auto root = parse_tree::parse<Grammar, Selector>(input);
    visit(root);
    parse_tree::print_dot(std::cout, *root);
    return root;
}
