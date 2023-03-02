# CTL Model Checker

The goal of this project is to implement CTL model checker using aproach introduced in the science paper ([CTL Model Checking Based on Forward State Traversal](https://citeseerx.ist.psu.edu/viewdoc/download?doi=10.1.1.469.1817&rep=rep1&type=pdf)).

## How to run
Tested on Ubuntu 22.04.1 LTS.

### Install required packages
```bash
sudo apt-get update
sudo apt-get install git clang-15 cmake
```

### Clone repository
```bash
git clone https://gitlab.unige.ch/Nikolay.Chikin/ctl-model-checker.git
```

### Build project
```bash
cd ctl-model-checker
mkdir build && cd build
export CXX=clang++-15
cmake -DCMAKE_BUILD_TYPE=Release ..
cmake --build . && cmake --install .
```

### Run executable
```bash
./../programs/CTL-model-checker --help
```
