#pragma once
#include <vector>
#include "DFA.h"

namespace RegularExpression {

NFA Create(std::string expression);
NFA Create(std::vector<std::string> expressions);

};