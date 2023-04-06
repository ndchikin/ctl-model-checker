#include <fstream>
#include <parsers/fsm/fsm.h>

#define STATE_BITS (sizeof(state_t) * 8)

uint64_t FSM::instances = 0;

void FSM::init_sylvan() {
    lace_start(0, 0);
    sylvan::sylvan_set_sizes(UINT64_C(1) << 16, UINT64_C(1) << 28, UINT64_C(1) << 16, UINT64_C(1) << 28);
    sylvan::sylvan_init_package();
    sylvan::sylvan_init_bdd();
}

void FSM::free_sylvan() {
    sylvan::sylvan_quit();
}

void FSM::init_vars() {
    for (var_t i = 0; i < values.size(); ++i) {
        if (i % 2 == 0) {
            state_variables.add(i);
        }
        relation_variables.add(i);
    }
}

void FSM::read(const std::string& path) {
    state_t n;
    state_t m;
    state_t b;
    state_t l = 0;
    std::string s;
    std::string beg;
    std::string end;
    std::fstream file(path, std::ios_base::in);
    file >> n;
    for (size_t i = 0; i < n; ++i) {
        file >> s;
        state_to_index[s] = i;
    }
    file >> n;
    for (size_t i = 0; i < n; ++i) {
        file >> s;
        add_initial_state(state_to_index.at(s));
    }
    file >> n;
    for (size_t i = 0; i < n; ++i) {
        file >> beg;
        file >> end;
        add_edge(state_to_index.at(beg), state_to_index.at(end));
    }
    file >> n;
    for (size_t i = 0; i < n; ++i) {
        file >> s;
        b = state_to_index.at(s);
        file >> m;
        for (size_t j = 0; j < m; ++j) {
            file >> s;
            auto it = label_to_index.try_emplace(s, l);
            if (it.second) {
                ++l;
            }
            add_label(b, it.first->second);
        }
    }
}

void FSM::add_initial_state(state_t initial_state) {
    for (size_t i = 0; i < STATE_BITS; ++i) {
        values[i] = (initial_state >> (STATE_BITS - 1 - i)) & 1;
    }
    initial_states = initial_states.UnionCube(state_variables, values);
}

void FSM::add_edge(state_t beg, state_t end) {
    uint8_t shift;
    for (size_t i = 0; i < STATE_BITS; ++i) {
        shift = STATE_BITS - 1 - i;
        values[i * 2] = (beg >> shift) & 1;
        values[i * 2 + 1] = (end >> shift) & 1;
    }
    transition_relation = transition_relation.UnionCube(relation_variables, values);
}

void FSM::add_label(state_t state, state_t label) {
    uint8_t shift;
    for (size_t i = 0; i < STATE_BITS; ++i) {
        shift = STATE_BITS - 1 - i;
        values[i * 2] = (state >> shift) & 1;
        values[i * 2 + 1] = (label >> shift) & 1;
    }
    label_relation = label_relation.UnionCube(relation_variables, values);
}

sylvan::Bdd FSM::next(sylvan::Bdd cur) {
    return cur.RelNext(transition_relation, all_variables);
}

FSM::FSM(const std::string& path) : all_variables(sylvan::sylvan_false), values(STATE_BITS * 2) {
    if (instances == 0) {
        init_sylvan();
    }
    ++instances;
    init_vars();
    read(path);
};

void FSM::print() {
    auto f = std::fopen("initial_states.txt", "w");
    initial_states.PrintDot(f);
    std::fclose(f);
    f = std::fopen("transition_relation.txt", "w");
    transition_relation.PrintDot(f);
    std::fclose(f);
    f = std::fopen("label_relation.txt", "w");
    label_relation.PrintDot(f);
    std::fclose(f);
}

FSM::~FSM() {
    --instances;
    if (instances == 0) {
        free_sylvan();
    }
};
