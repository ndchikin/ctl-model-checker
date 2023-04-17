#include <checker/checker.h>
#include <parsers/ast/grammar.h>

#include <fstream>
#include <stdexcept>
#include <utility>

using fp_func_t = const std::function<sylvan::Bdd(const sylvan::Bdd&)>&;

sylvan::Bdd fp(fp_func_t f, sylvan::Bdd& Z) {
    while (std::exchange(Z, f(Z)) != Z);
    return Z;
}

sylvan::Bdd lfp(fp_func_t f) {
    sylvan::Bdd Z(sylvan::sylvan_false);
    return fp(f, Z);
}

sylvan::Bdd gfp(fp_func_t f) {
    sylvan::Bdd Z(sylvan::sylvan_true);
    return fp(f, Z);
}

sylvan::Bdd compute_node(const std::unique_ptr<node_t>& n, const FSM& fsm) {
    if (n->is_type<True>()) {
        return sylvan::sylvan_true;
    } else if (n->is_type<False>()) {
        return sylvan::sylvan_false;
    } else if (n->is_type<Atom>()) {
        return fsm.label_to_states(n->string());
    } else if (n->is_type<Not>()) {
        return !compute_node(n->children.front(), fsm);
    } else if (n->is_type<And>()) {
        sylvan::Bdd states(sylvan::sylvan_true);
        for (auto& c : n->children) {
            states &= compute_node(c, fsm);
            if (states.isZero()) {
                break;
            }
        }
        return states;
    } else if (n->is_type<EX>()) {
        return fsm.pre(compute_node(n->children.front(), fsm));
    } else if (n->is_type<EG>()) {
        const auto f = compute_node(n->children.front(), fsm);
        return gfp([&f, &fsm](const auto& Z) { return f & fsm.pre(Z); });
    } else if (n->is_type<EU>()) {
        const auto g = compute_node(n->children.front(), fsm);
        const auto f = compute_node(n->children.back(), fsm);
        return lfp([&g, &f, &fsm](const auto& Z) { return f | (g & fsm.pre(Z)); });
    } else if (n->is_type<p0>()) {
        return fsm.initial_state;
    } else if (n->is_type<Eq>()) {
        const auto states0 = compute_node(n->children.front(), fsm);
        const auto states1 = compute_node(n->children.back(), fsm);
        return states0 == states1 ? sylvan::sylvan_true : sylvan::sylvan_false;
    } else if (n->is_type<Img>()) {
        return fsm.img(compute_node(n->children.front(), fsm));
    } else if (n->is_type<FU>()) {
        const auto p = compute_node(n->children.front(), fsm);
        const auto q = compute_node(n->children.back(), fsm);
        return lfp([&p, &q, &fsm](const auto& Z) { return p | fsm.img(Z & q); });
    } else if (n->is_type<FG>()) {
        const auto p = compute_node(n->children.front(), fsm);
        const auto q = compute_node(n->children.back(), fsm);
        const auto tmp = lfp([&p, &q, &fsm](const auto& Z) { return p | fsm.img(Z & q); }) & q;
        return gfp([&tmp, &fsm](const auto& Z) { return tmp & fsm.img(Z); });
    } else {
        throw std::runtime_error("Don't know how to compute node with type: " + static_cast<std::string>(n->type));
    }
}

void evaluate_formula(const AST& ast, const FSM& fsm, const std::string& path) {
    auto bdd = compute_node(ast.root, fsm);
    std::fstream file(path, std::ios_base::out);
    file << bdd.isOne();
}
