#include <iostream>
#include <sylvan_obj.hpp>

void init_sylvan() {
    lace_start(0, 0);
    sylvan::sylvan_set_sizes(1 << 10, UINT64_C(1) << 28, 1 << 10, UINT64_C(1) << 28);
    sylvan::sylvan_init_package();
    sylvan::sylvan_init_bdd();
}

void free_sylvan() {
    sylvan::sylvan_quit();
}

class BDD {
public:
    sylvan::Bdd initial_states;
    sylvan::Bdd transition_relation;
    sylvan::BddSet state_variables;
    sylvan::BddSet relation_variables;
    sylvan::BddSet all_variables;
    std::vector<uint8_t> values;

public:
    BDD() : all_variables(sylvan::sylvan_false), values(sizeof(uint64_t) * 8 * 2) {
        for (uint32_t i = 0; i < values.size(); ++i) {
            if (i % 2 == 0) {
                state_variables.add(i);
            }
            relation_variables.add(i);
        }
    };

    void add(uint64_t initial_state) {
        for (size_t i = 0; i < 64; ++i) {
            values[i] = (initial_state >> (63 - i)) & 1;
        }
        initial_states = initial_states.UnionCube(state_variables, values);
    }

    void add(uint64_t beg, uint64_t end) {
        uint8_t shift;
        for (size_t i = 0; i < 64; ++i) {
            shift = 63 - i;
            values[i * 2] = (beg >> shift) & 1;
            values[i * 2 + 1] = (end >> shift) & 1;
        }
        transition_relation = transition_relation.UnionCube(relation_variables, values);
    }

    sylvan::Bdd next(sylvan::Bdd cur) {
        return cur.RelNext(transition_relation, all_variables);
    }

    void print() {
        auto f = std::fopen("dot.txt", "w");
        initial_states.PrintDot(f);
        std::fclose(f);
    }
};

int main() {
    init_sylvan();
    BDD bdd;
    for (size_t i = 0; i < 10000000; ++i) {
        if (i % 1000000 == 0) {
            std::cout << i << '\n';
        }
        if (i % 13 == 0) {
            bdd.add(i);
        }
    }
    bdd.print();
    std::cout << bdd.initial_states.NodeCount() << '\n';
    free_sylvan();
    return 0;
}
