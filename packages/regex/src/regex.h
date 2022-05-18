#pragma once
#include "DFA.h"
#include <vector>

namespace regex {

DFA Create(std::string expression);
DFA Create(std::vector<std::string> expressions);

};