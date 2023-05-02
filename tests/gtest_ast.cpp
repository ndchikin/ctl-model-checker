#include <gtest/gtest.h>
#include <parsers/ast/ast.h>
#include <parsers/ast/common.h>
#include <parsers/ast/grammar.h>

#include <string>

template <typename T>
node_ptr make_node(const char* str) {
    auto node = std::make_unique<node_t>();
    node->set_type<T>();
    node->m_begin.data = str;
    node->m_end.data = str + std::strlen(str);
    return node;
}

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
        if (!compare_ast(n1->children[i], n2->children[i])) {
            return false;
        }
    }
    return true;
}

TEST(AST, DoubleNot) {
    AST ast("¬¬f");
    auto f = make_node<Atom>("f");
    auto p = make_node<p0>();
    auto and0 = make_node<And>();
    auto not0 = make_node<Not>();
    auto eq = make_node<Eq>();
    auto false0 = make_node<False>();
    add_child(and0, p);
    add_child(and0, f);
    add_child(eq, and0);
    add_child(eq, false0);
    add_child(not0, eq);
    EXPECT_TRUE(compare_ast(ast.root, not0));
}

TEST(AST, Example) {
    AST ast("E[q U ¬AX(f)]");
    auto q = make_node<Atom>("q");
    auto f = make_node<Atom>("f");
    auto p = make_node<p0>();
    auto img = make_node<Img>();
    auto fu = make_node<FU>();
    auto and0 = make_node<And>();
    auto not0 = make_node<Not>();
    auto not1 = make_node<Not>();
    auto eq = make_node<Eq>();
    auto false0 = make_node<False>();
    add_child(fu, p);
    add_child(fu, q);
    add_child(img, fu);
    add_child(not0, f);
    add_child(and0, img);
    add_child(and0, not0);
    add_child(eq, and0);
    add_child(eq, false0);
    add_child(not1, eq);
    EXPECT_TRUE(compare_ast(ast.root, not1));
}

TEST(AST, ExampleFromPaper) {
    AST ast("AG(req ⇒ AFack)");
    auto p = make_node<p0>();
    auto req = make_node<Atom>("req");
    auto ack = make_node<Atom>("ack");
    auto fg = make_node<FG>();
    auto fu = make_node<FU>();
    auto true0 = make_node<True>();
    auto false0 = make_node<False>();
    auto and0 = make_node<And>();
    auto not0 = make_node<Not>();
    auto eq = make_node<Eq>();
    add_child(fu, p);
    add_child(fu, true0);
    add_child(and0, fu);
    add_child(and0, req);
    add_child(not0, ack);
    add_child(fg, and0);
    add_child(fg, not0);
    add_child(eq, fg);
    add_child(eq, false0);
    EXPECT_TRUE(compare_ast(ast.root, eq));
}
