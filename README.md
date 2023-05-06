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

## Example run
In folder [example](example) there is result of executing [run.sh](example/run.sh) command.
