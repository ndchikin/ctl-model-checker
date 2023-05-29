../programs/ctl-model-checker --ctl-formula formula.txt --fsm-model model.txt --bdd-stats stats.txt --ast-dot ast.dot > output.txt
dot -Tsvg ast.dot -o ast.svg
