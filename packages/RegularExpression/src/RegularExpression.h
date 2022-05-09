#pragma once
#include "DFA.h"
#include <vector>

namespace RegularExpression {

NFA Create(std::string expression);
NFA Create(std::vector<std::string> expressions);

};