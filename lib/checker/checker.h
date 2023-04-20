#pragma once

#include <parsers/ast/ast.h>
#include <parsers/fsm/fsm.h>

bool evaluate_formula(const AST& ast, const FSM& fsm);
