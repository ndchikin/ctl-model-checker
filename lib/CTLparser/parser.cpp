#include <CTLparser/parser.h>
#include <iostream>
#include <tao/pegtl.hpp>
#include <tao/pegtl/contrib/parse_tree.hpp>
#include <tao/pegtl/contrib/parse_tree_to_dot.hpp>

using namespace tao::pegtl;

struct True : utf8::string<'t', 'r', 'u', 'e'> {};
struct False : utf8::string<'f', 'a', 'l', 's', 'e'> {};

struct Boolean : sor<True, False> {};

struct Atom : identifier {};

struct Space : utf8::one<' '> {};
struct Not : utf8::one<U'¬'> {};
struct And : pad<utf8::one<U'∧'>, Space> {};
struct Or : pad<utf8::one<U'∨'>, Space> {};
struct Imply : pad<utf8::one<U'⇒'>, Space> {};
struct IFF : pad<utf8::one<U'⇔'>, Space> {};

struct RBL : utf8::one<'('> {};
struct RBR : utf8::one<')'> {};
struct SBL : utf8::one<'['> {};
struct SBR : utf8::one<']'> {};

struct A : utf8::one<'A'> {};
struct E : utf8::one<'E'> {};
struct X : utf8::one<'X'> {};
struct F : utf8::one<'F'> {};
struct G : utf8::one<'G'> {};
struct U : utf8::one<'U'> {};

struct P0;
struct P5;

struct Bracketed : seq<RBL, P5, RBR> {};

struct AX : seq<A, X, P0> {};
struct EX : seq<E, X, P0> {};
struct AF : seq<A, F, P0> {};
struct EF : seq<E, F, P0> {};
struct AG : seq<A, G, P0> {};
struct EG : seq<E, G, P0> {};

struct PU : pad<U, Space> {};
struct SFUFS : seq<SBL, P5, PU, P5, SBR> {};

struct AU : seq<A, SFUFS> {};
struct EU : seq<E, SFUFS> {};

struct TOP : sor<AX, EX, AF, EF, AG, EG, AU, EU> {};

struct P0 : sor<TOP, Boolean, Atom, Bracketed> {};
struct P1 : seq<opt<Not>, P0> {};
struct P2 : list<P1, And> {};
struct P3 : list<P2, Or> {};
struct P4 : list<P3, Imply> {};
struct P5 : list<P4, IFF> {};

struct Grammar : must<P5, eof> {};

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
    auto a = make_node<Node, And>();
    auto x = std::move(n->children.front());
    auto y = std::move(n->children.back());
    add_child(not1, y);
    add_child(a, x);
    add_child(a, not1);
    add_child(not0, a);
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

void parse_formula(const std::string& path) {
    file_input input(path);
    const auto root = parse_tree::parse<Grammar, Selector>(input);
    parse_tree::print_dot(std::cout, *root);
}
