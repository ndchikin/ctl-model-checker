#include <cstdint>
#include <cstdlib>
#include <sylvan_obj.hpp>
#include <unordered_map>

void init_sylvan() {
    lace_start(0, 0);
    sylvan::sylvan_set_sizes(1<<10, 1<<12, 1<<10, 1<<12);
    sylvan::sylvan_init_package();
    sylvan::sylvan_init_bdd();
}

void free_sylvan() {
    sylvan::sylvan_quit();
}

// class BDD {
// private:
//     sylvan::Bdd initial;
//     sylvan::BddSet initial;

//     uint64_t to_index(const std::string& state);

// public:
//     void add_initial(const std::string& state) {
//         auto index = to_index(state);

//     }
// }

int main() {
    init_sylvan();
    std::vector<uint32_t> vars({0, 1, 2, 3, 4, 5, 6, 7, 8});
    std::vector<uint8_t> vals1({1, 0, 1, 0, 1, 0, 1, 0, 1});
    std::vector<uint8_t> vals2({0, 1, 0, 1, 0, 1, 0, 1, 0});
    auto set = sylvan::BddSet::fromVector(vars);
    sylvan::Bdd bdd;
    bdd = bdd.UnionCube(set, vals1);
    bdd = bdd.UnionCube(set, vals2);
    auto f = std::fopen("dot.txt", "w");
    bdd.PrintDot(f);
    std::fclose(f);
    free_sylvan();
    return 0;
}
