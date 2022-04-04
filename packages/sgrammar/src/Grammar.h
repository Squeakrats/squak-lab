#pragma once
#include <set>
#include <string>
#include <map>
#include <vector>
#include "Parser.h"

class Grammar {
	using RuleTable = std::vector<std::map<std::string, size_t>>;
public:
	std::map<std::string, std::vector<std::vector<std::string>>> productions{};

	std::string Root();
	std::set<std::string> Terminals();
	RuleTable Rules();
	std::set<std::string> First(std::string symbol);
	std::set<std::string> First(std::string symbol, size_t index);

	static Grammar Create(std::string source);
};