#pragma once

#include <parsers/ast/ast.h>
#include <parsers/fsm/fsm.h>

std::vector<std::string> evaluate_formula(const std::unique_ptr<node_t>& ast_root, FSM& fsm);
void write_states(const std::string& path, const std::vector<std::string>& states);
