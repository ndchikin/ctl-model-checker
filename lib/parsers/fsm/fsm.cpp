#include <parsers/fsm/fsm.h>

#include <fstream>
#include <sstream>

#define TABLE_RATIO (1)
#define INITIAL_RATIO (8)

size_t getMaxMemory() {
    const auto pages = sysconf(_SC_PHYS_PAGES);
    const auto page_size = sysconf(_SC_PAGE_SIZE);
    return pages * page_size;
}

uint64_t SylvanIniter::instances = 0;

SylvanIniter::SylvanIniter() {
    if (instances == 0) {
        lace_start(0, 0);
        sylvan::sylvan_set_limits(getMaxMemory(), TABLE_RATIO, INITIAL_RATIO);
        sylvan::sylvan_init_package();
        sylvan::sylvan_init_bdd();
    }
    ++instances;
}

SylvanIniter::~SylvanIniter() {
    --instances;
    if (instances == 0) {
        sylvan::sylvan_quit();
        lace_stop();
    }
}

void FSM::init_vars() {
    for (var_t i = 0; i < values.size(); ++i) {
        if (i % 2 == 0) {
            state_variables.add(i);
        }
        relation_variables.add(i);
    }
}

void FSM::init() {
    init_vars();
    set_initial_state();
}

void FSM::read_fsm(std::istream& input) {
    state_t n;
    state_t m;
    std::string s;
    std::string beg;
    std::string end;
    input >> n;
    for (size_t i = 0, l = 0; i < n; ++i) {
        input >> s;
        state_to_index[s] = i;
        input >> m;
        for (size_t j = 0; j < m; ++j) {
            input >> s;
            auto it = label_to_index.try_emplace(s, l);
            if (it.second) {
                ++l;
            }
            add_label(i, it.first->second);
        }
    }
    input >> n;
    for (size_t i = 0; i < n; ++i) {
        input >> beg;
        input >> end;
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
    encode(label, state);
    label_to_state = label_to_state.UnionCube(relation_variables, values);
}

FSM::FSM(const std::string& path) : all_variables(sylvan::sylvan_false), values(STATE_BITS * 2) {
    init();
    std::fstream input(path, std::ios_base::in);
    read_fsm(input);
};

FSM::FSM(const char* str) : all_variables(sylvan::sylvan_false), values(STATE_BITS * 2) {
    init();
    std::stringstream input(str);
    read_fsm(input);
};

sylvan::Bdd FSM::pre(const sylvan::Bdd& states) const {
    return states.RelPrev(state_to_state, all_variables);
}

sylvan::Bdd FSM::img(const sylvan::Bdd& states) const {
    return states.RelNext(state_to_state, all_variables);
}

sylvan::Bdd FSM::label_to_states(const std::string& label) const {
    auto it = label_to_index.find(label);
    if (it == label_to_index.end()) {
        return sylvan::sylvan_false;
    } else {
        encode(it->second);
        auto label_bdd = sylvan::Bdd::bddCube(state_variables, values);
        return label_bdd.RelNext(label_to_state, all_variables);
    }
}

void FSM::print() const {
    auto* f = std::fopen("initial_states.txt", "w");
    initial_state.PrintDot(f);
    (void)std::fclose(f);
    f = std::fopen("transition_relation.txt", "w");
    state_to_state.PrintDot(f);
    (void)std::fclose(f);
    f = std::fopen("label_relation.txt", "w");
    label_to_state.PrintDot(f);
    (void)std::fclose(f);
}
