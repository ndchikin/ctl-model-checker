#pragma once

#include <sylvan_obj.hpp>
#include <unordered_map>

typedef uint32_t var_t;
typedef uint32_t state_t;

class FSM {
private:
    sylvan::Bdd state_to_state;
    sylvan::Bdd state_to_label;

    sylvan::BddSet state_variables;
    sylvan::BddSet relation_variables;
    sylvan::BddSet all_variables;

    mutable std::vector<uint8_t> values;

    std::unordered_map<std::string, state_t> state_to_index;
    std::unordered_map<std::string, state_t> label_to_index;

    static uint64_t instances;

    void init_sylvan();
    void free_sylvan();
    void init_vars();
    void read_fsm(const std::string& path);

    void encode(state_t state) const;
    void encode(state_t state1, state_t state2) const;
    void set_initial_state();
    void add_edge(state_t beg, state_t end);
    void add_label(state_t state, state_t label);

public:
    sylvan::Bdd initial_state;

    FSM(const std::string& path);

    sylvan::Bdd pre(sylvan::Bdd states) const;
    sylvan::Bdd img(sylvan::Bdd states) const;
    sylvan::Bdd label_to_states(const std::string& label) const;
    void print() const;

    ~FSM();
};
