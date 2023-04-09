#pragma once

#include <parsers/ast/common.h>
#include <parsers/ast/grammar.h>
#include <tao/pegtl/contrib/parse_tree.hpp>

std::unique_ptr<node_t> copy(const std::unique_ptr<node_t>& n);
std::unique_ptr<node_t> deep_copy(const std::unique_ptr<node_t>& n);
void fold(std::unique_ptr<node_t>& n);
void replace_not(std::unique_ptr<node_t>& n);
void replace_or(std::unique_ptr<node_t>& n);
void replace_imply(std::unique_ptr<node_t>& n);
void replace_iff(std::unique_ptr<node_t>& n);

struct Rewrite : parse_tree::apply<Rewrite> {
    static void transform(std::unique_ptr<node_t>& n);
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
    static void transform(std::unique_ptr<node_t>& n);
};

template<>
struct Selector<AF> : std::true_type {
    static void transform(std::unique_ptr<node_t>& n);
};

template<>
struct Selector<EF> : std::true_type {
    static void transform(std::unique_ptr<node_t>& n);
};

template<>
struct Selector<AG> : std::true_type {
    static void transform(std::unique_ptr<node_t>& n);
};

template<>
struct Selector<AU> : std::true_type {
    static void transform(std::unique_ptr<node_t>& n);
};
