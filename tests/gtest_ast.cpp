#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <parsers/ast/ast.h>
#include <parsers/ast/grammar.h>
#include <parsers/ast/selector.h>
#include <parsers/ast/visitors.h>

#include <string>

using namespace std::literals::string_literals;

bool compare_node(const node_ptr& n1, const node_ptr& n2) {
    if (n1->type == n2->type) {
        if (n1->children.size() == n2->children.size()) {
            if (n1->has_content() == n2->has_content()) {
                return !n1->has_content() || n1->string_view() == n2->string_view();
            }
        }
    }
    return false;
}

bool compare_ast(const node_ptr& n1, const node_ptr& n2) {
    if (!compare_node(n1, n2)) {
        return false;
    }
    for (size_t i = 0; i < n1->children.size(); ++i) {
        if (!compare_node(n1->children[i], n2->children[i])) {
            return false;
        }
    }
    return true;
}

void set_content(node_ptr& n, const std::string& c) {
    n->m_begin.data = c.c_str();
    n->m_end.data = c.c_str() + c.size();
}

// struct Not : utf8::one<U'¬'> {};
// struct And : pad<utf8::one<U'∧'>, Space> {};
// struct Or : pad<utf8::one<U'∨'>, Space> {};
// struct Imply : pad<utf8::one<U'⇒'>, Space> {};
// struct IFF : pad<utf8::one<U'⇔'>, Space> {};

// struct AX : seq<A, X, P0> {};
// struct AF : seq<A, F, P0> {};
// struct EF : seq<E, F, P0> {};
// struct AG : seq<A, G, P0> {};
// struct AU : seq<A, SFUFS> {};

// struct p0;
// struct Eq;
// struct Img;
// struct FU;
// struct FG;


TEST(AST, DoubleNot) {
    memory_input input_source("¬¬f", "");
    const auto ast_source = parse_tree::parse<Grammar, node_t, Selector>(input_source);
    std::string s_f("f");
    auto f = make_node<Atom>();
    set_content(f, s_f);
    const auto ast_target = make_node<p0>();
    ast_target->type = "";
    ast_target->children.push_back(std::move(f));
    EXPECT_TRUE(compare_ast(ast_source, ast_target));
}