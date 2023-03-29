#include <sylvan_obj.hpp>
#include <unordered_map>

typedef uint32_t var_t;
typedef uint32_t state_t;

class FSM {
private:
    sylvan::Bdd initial_states;
    sylvan::Bdd transition_relation;
    sylvan::Bdd label_relation;

    sylvan::BddSet state_variables;
    sylvan::BddSet relation_variables;
    sylvan::BddSet all_variables;

    std::vector<uint8_t> values;

    std::unordered_map<std::string, state_t> state_to_index;
    std::unordered_map<std::string, state_t> label_to_index;

    static uint64_t instances;

    void init_sylvan();
    void free_sylvan();
    void init_vars();
    void read(const std::string& path);

    void add_initial_state(state_t initial_state);
    void add_edge(state_t beg, state_t end);
    void add_label(state_t state, state_t label);
    sylvan::Bdd next(sylvan::Bdd cur);

public:
    FSM(const std::string& path);
    void print();
    ~FSM();
};
