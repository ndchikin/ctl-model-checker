#include <checker/checker.h>
#include <fstream>
#include <parsers/ast/grammar.h>

sylvan::Bdd compute_node(const std::unique_ptr<node_t>& n, FSM& fsm) {
    if (n->template is_type<Atom>()) {
        void(1);
    } else if (n->template is_type<Not>()) {
        return !compute_node(n->children.front(), fsm);
    } else if (n->template is_type<And>()) {
        sylvan::Bdd states;
        for (auto& c : n->children) {
            states &= compute_node(c, fsm);
        }
        return states;
    }
    return compute_node(n, fsm);
}

std::vector<std::string> evaluate_formula(const std::unique_ptr<node_t>& ast_root, FSM& fsm) {
    auto bdd = compute_node(ast_root, fsm);
    return std::vector<std::string>();
}

void write_states(const std::string& path, const std::vector<std::string>& states) {
    std::fstream file(path, std::ios_base::out);
    for (size_t i = 0; i < states.size(); ++i) {
        file << states[i];
        if (i < states.size() - 1) {
            file << ' ';
        }
    }
    file << '\n';
}
