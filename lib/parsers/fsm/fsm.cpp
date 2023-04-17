#include <fstream>
#include <parsers/fsm/fsm.h>

#define MIN_TABLESIZE (UINT32_C(1) << 16)
#define MAX_TABLESIZE (UINT32_C(1) << 28)
#define MIN_CHACHESIZE (UINT32_C(1) << 16)
#define MAX_CHACHESIZE (UINT32_C(1) << 28)
#define STATE_BITS (sizeof(state_t) * 8)

uint64_t FSM::instances = 0;

void FSM::init_sylvan() {
    lace_start(0, 0);
    sylvan::sylvan_set_sizes(MIN_TABLESIZE, MAX_TABLESIZE, MIN_CHACHESIZE, MAX_CHACHESIZE);
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

void FSM::read_fsm(const std::string& path) {
    state_t n;
    state_t m;
    std::string s;
    std::string beg;
    std::string end;
    std::fstream file(path, std::ios_base::in);
    file >> n;
    for (size_t i = 0, l = 0; i < n; ++i) {
        file >> s;
        state_to_index[s] = i;
        file >> m;
        for (size_t j = 0; j < m; ++j) {
            file >> s;
            auto it = label_to_index.try_emplace(s, l);
            if (it.second) {
                ++l;
            }
            add_label(i, it.first->second);
        }
    }
    file >> n;
    for (size_t i = 0; i < n; ++i) {
        file >> beg;
        file >> end;
        add_edge(state_to_index.at(beg), state_to_index.at(end));
    }
}

void FSM::encode(state_t state) const {
    uint8_t shift;
    for (size_t i = 0; i < STATE_BITS; ++i) {
        shift = STATE_BITS - 1 - i;
        values[i] = (state >> shift) & 1;
    }
}

void FSM::encode(state_t beg, state_t end) const {
    uint8_t shift;
    for (size_t i = 0; i < STATE_BITS; ++i) {
        shift = STATE_BITS - 1 - i;
        values[i * 2] = (beg >> shift) & 1;
        values[i * 2 + 1] = (end >> shift) & 1;
    }
}

void FSM::set_initial_state() {
    encode(0);
    initial_state = sylvan::Bdd::bddCube(state_variables, values);
}

void FSM::add_edge(state_t beg, state_t end) {
    encode(beg, end);
    state_to_state = state_to_state.UnionCube(relation_variables, values);
}

void FSM::add_label(state_t state, state_t label) {
    encode(state, label);
    state_to_label = state_to_label.UnionCube(relation_variables, values);
}

FSM::FSM(const std::string& path) : all_variables(sylvan::sylvan_false), values(STATE_BITS * 2) {
    if (instances == 0) {
        init_sylvan();
    }
    ++instances;
    init_vars();
    set_initial_state();
    read_fsm(path);
};

sylvan::Bdd FSM::pre(const sylvan::Bdd& states) const {
    return states.RelPrev(state_to_state, all_variables);
}

sylvan::Bdd FSM::img(const sylvan::Bdd& states) const {
    return states.RelNext(state_to_state, all_variables);
}

sylvan::Bdd FSM::label_to_states(const std::string& label) const {
    auto index = label_to_index.at(label);
    encode(index);
    auto label_bdd = sylvan::Bdd::bddCube(state_variables, values);
    return label_bdd.RelPrev(state_to_label, all_variables);
}

void FSM::print() const {
    auto* f = std::fopen("initial_states.txt", "w");
    initial_state.PrintDot(f);
    (void)std::fclose(f);
    f = std::fopen("transition_relation.txt", "w");
    state_to_state.PrintDot(f);
    (void)std::fclose(f);
    f = std::fopen("label_relation.txt", "w");
    state_to_label.PrintDot(f);
    (void)std::fclose(f);
}

FSM::~FSM() {
    --instances;
    if (instances == 0) {
        free_sylvan();
    }
};
