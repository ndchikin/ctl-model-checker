#pragma once

#include <parsers/ast/ast.h>
#include <parsers/fsm/fsm.h>

void evaluate_formula(const AST& ast, const FSM& fsm, const std::string& path);
bool evaluate_formula(const AST& ast, const FSM& fsm);
