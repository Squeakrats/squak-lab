#pragma once
#include <set>
#include <string>
#include <map>
#include <vector>
#include "Parser.h"

namespace BackusNaur {

class Grammar {
public:
	std::map<std::string, std::vector<std::vector<std::string>>> productions{};

	std::set<std::string> GetTerminals();

	static Grammar Create(std::string source);
};

};