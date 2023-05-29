# 2023-03-07

## Research

This week we researched suggested paper: [CTL Model Checking Based on Forward State Traversal](https://citeseerx.ist.psu.edu/viewdoc/download?doi=10.1.1.469.1817&rep=rep1&type=pdf). Normaly temporal operators in a CTL formula are evaluated with backward state traversal. The paper describes a way to speed up CTL model checking by conwerting temporal operators into forward traversal operators. According to experiments on large models this method shows significant evaluation time improvement compared to backward state traversal when many temporal operators in a CTL formula are converted into forward traversal operators and transition relation is partitioned.

## Architecture

![model checker architecture](arch.png)
Our CTL model checker takes a CTL formula and a model (FSM) as input. After a series of transformations and evaluation we get the initial states of the FSM that satisfy the property expressed by the CTL formula.

## General plan

* Tokenization (1 week)
* AST (1 week)
* Rewriting (1 week)
* Transition relation (1 week)
* Partitioning (2 weeks)
* BDD encoding (2 weeks)
* Formula evaluation (2 weeks)

# 2023-03-14

## Research

* Original paper
* CTL grammar
* [argparse](https://github.com/p-ranav/argparse) - Used for parsing CLI arguments.
* [PEGTL](https://github.com/taocpp/PEGTL) - Used for parsing CTL formula, building AST and AST rewriting.

## Ojective

* Rewrite temporal operators in CTL formula to forward trawersal operators where it's possible

## Result

* We have MVP for building AST for CTL formulas

## Next goal

* Rewrite temporal operators in AST to forward trawersal operators

## Who did what

* Jing - Presentation and research
* Nikolay - Programming

# 2023-03-21

## Result

* We have reduced set of operators in AST to {true, false, ¬, ∧, EX, EU, EG} (Nikolay)

# 2023-03-28

## Research

* [sylvan](https://github.com/trolando/sylvan) - multi-core binary decision diagrams with native support for image and preimage operations

## Ojective

* Fully understand BBDs. In particular how to partition transition relation
* Discover the best way to input FSM into our model checker (there are many different formats)

## Result

* We have completed the CTL formula parser. Now it can reproduce example from the reference paper

## Next goal

* Decide on the file format with the description of FSM and how to read it

# 2023-04-04

## Research

* [fmt](https://github.com/fmtlib/fmt.git) - used for printing C++ std containers
* Papers about BDDs, logic and model checking
  * [Sylvan: multi-core framework for decision diagrams](https://link.springer.com/content/pdf/10.1007/s10009-016-0433-2.pdf?pdf=button)
  * [Generating BDDs for Symbolic Model Checking in CCS](https://link.springer.com/content/pdf/10.1007/3-540-55179-4_20.pdf)
  * [A Comparative Study of BDD Packages for Probabilistic Symbolic Model Checking](https://www.tvandijk.nl/pdf/2015setta.pdf)

## Result

* We have an MVP for the FSM parser and building the basic BDD structures

## Next goal

* Decide on the file format with the description of FSM and how to read it
* Understand how to partition transition relation

## Who did what

* Jing - Presentation and research
* Nikolay - Programming

# 2023-04-11

## Research

* Existing model checkers and their input formats
  * [NuSMV](https://nusmv.fbk.eu/)
  * [mCRL2](https://mcrl2.org/web/user_manual/language_reference/lts.html)

## Result

* FSM parser and required BDD structures
* Specification of the FSM input format:
  1. Number of states n, the first state is the initial state
  2. Next n lines: state, number of labels, list of labels
  3. Number of edges m
  4. Next m lines: state, state
* Example of FSM input format:

      3
      S0 2 begin start
      S1 0
      S2 1 finish
      2
      S0 S1
      S1 S2

* First steps for formula evaluation

## Next goal

* Finish the code for formula evaluation

# 2023-04-18

## Research
* In total we have to implement evaluation for 13 types of AST nodes:
  * True
  * False
  * Atom
  * Not
  * And
  * EX
  * EG
  * EU
  * p0
  * Eq
  * Img
  * FU
  * FG

## Result

* Previous week we implemented evaluation for:
  * True
  * False
  * Atom
  * Not
  * And
  * p0
  * Eq
  * Img
* This week we implemented evaluation for:
  * EX
  * EG
  * EU
  * FU
  * FG

## Next goal

* Add tests and find all bugs, if any

# 2023-04-25

## Research

* [googletest](https://github.com/google/googletest.git) - used for testing

## Result

* We have a working model checker
* At the moment it doesn't partition the transition relation, but it works correctly
* More than 20 tests to evaluate various CTL formulas against several different models
* Found several bugs thanks to tests

## Next goal

* Implement partitioning for transition relation
* Various performance comparisons

## Who did what

* Jing - Presentation and research
* Nikolay - Programming

# 2023-05-02

## Research

* Theory understanding

## Result

* No new code, just brin work

## Next goal

* Add human readable test (human friendly, understandable model and properties to check)
* Implement partitioning for transition relation

# 2023-05-09

## Research

* GitLab CI/CD
* Paper about transition relation partitioning: [Representing Circuits More Efficiently in Symbolic Model Checking](https://www.cs.cmu.edu/~emc/papers/Conference%20Papers/Representing%20Circuits%20More%20Efficiently%20in%20Symbolic%20Model%20Checking.pdf)

## Result

* Automated GitLab CI/CD pipeline for build, test and packaging tasks
* Understanding the partitioning of the transition relationship
* Understanding of the fact that in our case there is alwaus one partition and we have no use case or reason to implement partitioning
* Preparation for performance testing (option to turn off forward state traversal and option to write BDD usage statistic)

## Next goal

* Add a human-readable test (human-friendly, understandable model and properties to check)
* Perhaps rethink model format to make partitioning actually usable
* Performance tests

# 2023-05-16

## Research

* [ITS-Tools](https://lip6.github.io/ITSTools-web/tpn.html): it seems that you can use this tool to convert a time petri net to a kripke structure, and to check the CTL properties relative to the time petri net. Here is the plan of how to make a performance test

## Result

* Human-readable tests (example from wikipedia and microwave from lectures)
* Option to save AST in dot format

## Next goal

* Make performance test
* Make final presentation

# 2023-05-23

# 2023-05-30

## Research

* [lcov](https://github.com/linux-test-project/lcov): Used for test coverage calculation

## Result

* Test coverage report in GitLab CI
* Better documentation in README
* Final presentation

## Who did what

* Jing - Presentation and research
* Nikolay - Programming
