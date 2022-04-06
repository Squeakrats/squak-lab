#pragma once
#include <set>
#include <string>
#include <map>
#include <vector>
#include "Parser.h"

class Grammar {
private:
	std::map<std::string, AST::Production> productions{};

public:
	AST::Grammar ast{};

	std::set<std::string> Terminals();
	std::map<size_t, std::set<std::string>> Rules(std::string symbol);
	std::set<std::string> First(std::string symbol);
	std::set<std::string> First(std::string symbol, size_t index);

	static Grammar Create(std::string source);
};
