# CTL Model Checker

[![pipeline status](https://gitlab.unige.ch/Nikolay.Chikin/ctl-model-checker/badges/main/pipeline.svg)](https://gitlab.unige.ch/Nikolay.Chikin/ctl-model-checker/-/commits/main)

The goal of this project is to implement CTL model checker using aproach introduced in the paper ([CTL Model Checking Based on Forward State Traversal](https://citeseerx.ist.psu.edu/viewdoc/download?doi=10.1.1.469.1817&rep=rep1&type=pdf)).

## How to run

Tested on Ubuntu 22.04.1 LTS.

### Install required packages

```bash
sudo apt-get update
sudo apt-get install git clang-15 cmake ninja-build
```

### Clone repository

```bash
git clone --recursive git@gitlab.unige.ch:Nikolay.Chikin/ctl-model-checker.git
```

### Build project

```bash
cd ctl-model-checker
mkdir build && cd build
export CC=clang-15 CXX=clang++-15
cmake -G Ninja -DCMAKE_BUILD_TYPE=Release ..
cmake --build . && cmake --install .
```

### Run executable

```bash
./../programs/CTL-model-checker --help
```

```text
Usage: ctl-model-checker [--help] [--version] --ctl-formula VAR --fsm-model VAR [--use-bst] [--bdd-stats VAR]

Optional arguments:
  -h, --help            shows help message and exits
  -v, --version         prints version information and exits
  -c, --ctl-formula     file path to file with CTL formula [required]
  -f, --fsm-model       file path to file with FSM model [required]
  --use-bst             use backward state traversal, don't rewrites temporal operators using forward state traversal
  --bdd-stats           file path to file where to dump BDD usage statistic
```

## Example run

In folder [example](example) there is result of executing [run.sh](example/run.sh) command.

## Input formats

### CTL formula

[Syntax of CTL](https://en.wikipedia.org/wiki/Computation_tree_logic#Syntax_of_CTL)

### FSM model

In fact, the format of our FSM model is the same as that of the Kripke structure

#### Specification of the FSM input format

  1. Number of states n, the first state is the initial state
  2. Next n lines: state, number of labels, list of labels
  3. Number of edges m
  4. Next m lines: state, state

#### Example of FSM input format

```text
3
S0 2 begin start
S1 0
S2 1 finish
2
S0 S1
S1 S2
```
