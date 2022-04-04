#pragma once
#include <set>
#include <string>
#include <map>
#include <vector>
#include "Parser.h"

namespace BackusNaur {

class Grammar {
	using RuleTable = std::vector<std::map<std::string, size_t>>;
public:
	std::map<std::string, std::vector<std::vector<std::string>>> productions{};

	std::set<std::string> Terminals();
	RuleTable Rules();
	std::set<std::string> First(std::string symbol);
	std::set<std::string> First(std::string symbol, size_t index);

	static Grammar Create(std::string source);
};

};