# 2023-03-07
## Research
This week we researched suggested paper: [CTL Model Checking Based on Forward State Traversal](https://citeseerx.ist.psu.edu/viewdoc/download?doi=10.1.1.469.1817&rep=rep1&type=pdf). Normaly temporal operators in a CTL formula are evaluated with backward state traversal. The paper describes a way to speed up CTL model checking by conwerting temporal operators into forward traversal operators. According to experiments on large models this method shows significant evaluation time improvement compared to backward state traversal when many temporal operators in a CTL formula are converted into forward traversal operators and transition relation is partitioned.

## Architecture
![](arch.png)
Our CTL model checker takes a CTL formula and a model (FSM) as input. After a series of transformations and evaluation we get the initial states of the FSM that satisfy the property expressed by the CTL formula.

## General plan
* Tokenization (1 week)
* AST (1 week)
* Rewriting (1 week)
* Transition relation (1 week)
* Partitioning (2 weeks)
* BDD encoding (2 weeks)
* Formula evaluation (2 weeks)