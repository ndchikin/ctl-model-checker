#pragma once

#include <memory>
#include <sylvan_obj.hpp>
#include <unordered_map>

using var_t = uint32_t;
using state_t = uint32_t;

void sylvan_stats(const std::string& path);

class SylvanIniter {
private:
    static uint64_t instances;

public:
    SylvanIniter();
    ~SylvanIniter();
};

class IFSM {
public:
    virtual sylvan::Bdd get_initial_state() const = 0;
    virtual sylvan::Bdd pre(const sylvan::Bdd& states) const = 0;
    virtual sylvan::Bdd img(const sylvan::Bdd& states) const = 0;
    virtual sylvan::Bdd label_to_states(const std::string& label) const = 0;

    virtual ~IFSM() = default;
};

class FSMImplBase : public IFSM {
private:
    SylvanIniter initer;

    sylvan::Bdd initial_state;

    std::unordered_map<std::string, state_t> state_to_index;
    std::unordered_map<std::string, state_t> label_to_index;

    void init_vars();
    void set_initial_state();

    virtual void add_edge(state_t beg, state_t end) = 0;
    virtual void add_label(state_t state, state_t label) = 0;

protected:
    sylvan::BddSet state_variables;
    sylvan::BddSet relation_variables;
    sylvan::BddSet all_variables;

    mutable std::vector<uint8_t> values;

    void encode(state_t state) const;
    void encode(state_t beg, state_t end) const;

    void read_fsm(std::istream& input);

    sylvan::Bdd label_to_bdd(const std::string& label) const;

public:
    explicit FSMImplBase();

    sylvan::Bdd get_initial_state() const override;
};

class FSMImpl : public FSMImplBase {
private:
    sylvan::Bdd state_to_state;
    sylvan::Bdd label_to_state;

    void add_edge(state_t beg, state_t end) override;
    void add_label(state_t state, state_t label) override;

public:
    explicit FSMImpl(std::istream& input);

    sylvan::Bdd pre(const sylvan::Bdd& states) const override;
    sylvan::Bdd img(const sylvan::Bdd& states) const override;
    sylvan::Bdd label_to_states(const std::string& label) const override;
};

class PartitionedFSMImpl : public FSMImplBase {
private:
    std::vector<sylvan::Bdd> state_to_state;
    std::vector<sylvan::Bdd> label_to_state;

    void add_edge(state_t beg, state_t end) override;
    void add_label(state_t state, state_t label) override;

public:
    explicit PartitionedFSMImpl(std::istream& input);

    sylvan::Bdd pre(const sylvan::Bdd& states) const override;
    sylvan::Bdd img(const sylvan::Bdd& states) const override;
    sylvan::Bdd label_to_states(const std::string& label) const override;
};

class FSM : public IFSM {
private:
    std::unique_ptr<FSMImplBase> fsm_impl;

    void init_impl(std::istream& input, bool one_partition);

public:
    explicit FSM(const std::string& path, bool one_partition = false);
    explicit FSM(const char* str, bool one_partition = false);

    sylvan::Bdd get_initial_state() const override;
    sylvan::Bdd pre(const sylvan::Bdd& states) const override;
    sylvan::Bdd img(const sylvan::Bdd& states) const override;
    sylvan::Bdd label_to_states(const std::string& label) const override;
};
