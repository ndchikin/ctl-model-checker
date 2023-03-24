#include <cstdlib>
#include <sylvan_obj.hpp>

void init_sylvan() {
    lace_start(0, 0);
    sylvan::sylvan_set_sizes(1<<10, 1<<12, 1<<10, 1<<12);
    sylvan::sylvan_init_package();
    sylvan::sylvan_init_bdd();
}

void free_sylvan() {
    sylvan::sylvan_quit();
}

class BDD {
private:
    sylvan::Bdd initial_states;
    sylvan::Bdd transition_relation;
    sylvan::BddSet variables;
    sylvan::BddSet all_variables;
    std::vector<uint8_t> values;

public:
    BDD() : all_variables(sylvan::sylvan_false), values(sizeof(uint64_t) * 8 * 2) {
        for (uint32_t i = 0; i < values.size(); ++i) {
            variables.add(i);
        }
    };

    void add(uint64_t initial_state) {
        uint8_t shift;
        for (size_t i = 0; i < 64; ++i) {
            shift = 63 - i;
            values[i * 2] = (initial_state >> shift) & 1;
            values[i * 2 + 1] = 2;
        }
        initial_states = initial_states.UnionCube(variables, values);
    }

    void add(uint64_t beg, uint64_t end) {
        uint8_t shift;
        for (size_t i = 0; i < 64; ++i) {
            shift = 63 - i;
            values[i * 2] = (beg >> shift) & 1;
            values[i * 2 + 1] = (end >> shift) & 1;
        }
        transition_relation = transition_relation.UnionCube(variables, values);
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
    bdd.add(1);
    bdd.add(2);
    bdd.add(3);
    bdd.add(4);
    bdd.add(5);
    bdd.add(6);
    bdd.add(7);
    bdd.add(8);
    bdd.add(9);
    bdd.add(10);
    bdd.add(11);
    bdd.print();
    free_sylvan();
    return 0;
}
