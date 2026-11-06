# CTL Model Checker

This project implements a Computation Tree Logic (CTL) model checker based on the forward state traversal algorithm described in a research paper [1]. The algorithm is designed to check many realistic CTL properties with minimal use of backward state traversal, making it more efficient. The implementation combines this algorithm with Binary Decision Diagram (BDD)-based state traversal techniques. The model checker has been tested and verified to correctly check a wide range of CTL properties on several models. Take a look at the [logbook](logbook) for implementation details.

**Tech stack:** C++23, CMake + Ninja, Clang, [Sylvan](https://github.com/trolando/sylvan) (BDDs), [PEGTL](https://github.com/taocpp/PEGTL) (formula parser), [argparse](https://github.com/p-ranav/argparse), GoogleTest, code coverage, clang-format / clang-tidy, Docker.

University of Geneva project, spring 2023, by Nikolay Chikin and Jing Wen. The CI pipeline (Docker image, build, tests, coverage report, binary upload) ran on the university's GitLab; its configuration is kept in [.gitlab-ci.yml](.gitlab-ci.yml).

## What is CTL?

Computation Tree Logic (CTL) is a branching time logic, meaning it allows one to reason about the possible future paths a system could take. In CTL, one can express properties about the states of a system that must hold on all or some paths.

CTL is composed of state formulas and path formulas. State formulas are evaluated at a state in a model, and path formulas are evaluated along a path in a model. The state formulas include logical operations like AND, NOT, and path quantifiers like EX, AG.

CTL is widely used in model checking to formally verify properties of finite state systems.

## Why forward state traversal?

Forward state traversal in model checking is a technique that starts from a set of states and explores states using image operation. This approach is particularly beneficial when verifying Computation Tree Logic (CTL) properties. It allows for efficient checking of many realistic CTL properties without the need for more expensive backward state traversal, which uses pre-image operation. This approach has proven successful and efficient in verifying large Finite State Machines (FSMs), making it a practical method for large-scale industrial models.

## Tests

We check the correctness of evaluation for more than 20 CTL formulas and 4 different models using the standard backward state traversal algorithm and the forward state traversal algorithm from the paper. The models and formulas can be found in the [tests](tests) directory.

## Example run

The result of executing [run.sh](examples/run.sh) can be found in the [examples](examples) directory.

## Input formats

Examples can be found in the [tests](tests) directory.

### CTL formula

The syntax defined on [wikipedia](https://en.wikipedia.org/wiki/Computation_tree_logic#Syntax_of_CTL) is fully supported. However, this is a subset of the supported syntax. For example, you can put brackets and spaces everywhere. The definition of the CTL grammar is in the file [grammar.h](lib/parsers/ast/grammar.h).

### FSM model

The format of our FSM model is the same as that of a Kripke structure.

#### Specification of the FSM input format

  1. Number of states n, the first state is the initial state
  2. Next n lines: state, number of labels, list of labels
  3. Number of edges m
  4. Next m lines: state, state

## How to build

See also [Dockerfile](Dockerfile) and [.gitlab-ci.yml](.gitlab-ci.yml). The instructions below have been tested on Ubuntu 22.04.1 LTS.

### Install required packages

```bash
sudo apt-get update
sudo apt-get install git clang cmake ninja-build
```

### Clone repository

```bash
git clone --recursive https://github.com/ndchikin/ctl-model-checker.git
```

### Build project

```bash
cd ctl-model-checker
export CC=clang CXX=clang++
cmake -G Ninja -DCMAKE_BUILD_TYPE=Release -B build
cmake --build build --target install
```

### Run executable

```bash
./programs/ctl-model-checker --help
```

```text
Usage: ctl-model-checker [--help] [--version] --ctl-formula VAR [--fsm-model VAR] [--use-bst] [--bdd-stats VAR] [--ast-dot VAR]

Optional arguments:
  -h, --help            shows help message and exits
  -v, --version         prints version information and exits
  -c, --ctl-formula     file path to file with CTL formula [required]
  -f, --fsm-model       file path to file with FSM model
  --use-bst             use backward state traversal, don't rewrites temporal operators using forward state traversal
  --bdd-stats           file path to file where to dump BDD usage statistic
  --ast-dot             file path to file where to dump evaluated AST
```

## How to use our code?

We have 3 main classes in our program: [CLI](lib/parsers/cli/cli.h#L6) for parsing command-line arguments, [AST](lib/parsers/ast/ast.h#L13) to build abstract syntax tree and [FSM](lib/parsers/fsm/fsm.h#L20) to build the finite state machine. Formula evaluation can be performed with [evaluate_formula](lib/checker/checker.h#L6) function. You can see an example using all 4 of them in our [main](src/main.cpp#L8) function. It takes just 4 lines of code to perform model checking using our code!

## References

[1] H. Iwashita, T. Nakata, and F. Hirose, CTL model checking based on forward state traversal. 1996, pp. 82–87. doi: 10.5555/244522.244536.
