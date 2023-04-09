#pragma once

#include <parsers/ast/ast.h>
#include <parsers/fsm/fsm.h>

void evaluate_formula(const AST& ast, FSM& fsm, const std::string& path);
