#pragma once

#include <sylvan_obj.hpp>
#include <unordered_map>

using var_t = uint32_t;
using state_t = uint32_t;

#define STATE_BITS (sizeof(state_t) * 8)

class SylvanIniter {
private:
    static uint64_t instances;

public:
    SylvanIniter();
    ~SylvanIniter();
};

class FSM {
private:
    SylvanIniter initer;

    sylvan::Bdd state_to_state;
    sylvan::Bdd label_to_state;

    sylvan::BddSet state_variables;
    sylvan::BddSet relation_variables;
    sylvan::BddSet all_variables;

    mutable std::vector<uint8_t> values;

    std::unordered_map<std::string, state_t> state_to_index;
    std::unordered_map<std::string, state_t> label_to_index;

    void init_vars();
    void init();
    void read_fsm(std::istream& input);

    void encode(state_t state) const;
    void encode(state_t beg, state_t end) const;
    void set_initial_state();
    void add_edge(state_t beg, state_t end);
    void add_label(state_t state, state_t label);

public:
    sylvan::Bdd initial_state;

    explicit FSM(const std::string& path);
    explicit FSM(const char* str);

    sylvan::Bdd pre(const sylvan::Bdd& states) const;
    sylvan::Bdd img(const sylvan::Bdd& states) const;
    sylvan::Bdd label_to_states(const std::string& label) const;
};
