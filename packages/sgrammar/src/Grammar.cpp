#include "Grammar.h"

Grammar Grammar::Create(std::string source) {
	Grammar grammar{};
	for (auto production : Parser::Parse(source)) {
		grammar.productions.insert(production);
	}

	return grammar;
}

std::set<std::string> Grammar::Terminals() {
	std::set<std::string> terminals{};

	for (auto production : this->productions) {
		for (auto sequence : production.second) {
			for (auto symbol : sequence) {
				if (this->productions.find(symbol) == this->productions.end()) {
					terminals.insert(symbol);
				}
			}
		}
	}

	return terminals;
}

std::string Grammar::Root() {
	std::set<std::string> candidates{};

	for (auto production : this->productions) {
		candidates.insert(production.first);
	}

	for (auto production : this->productions) {
		for (auto sequence : production.second) {
			for (auto symbol : sequence) {
				candidates.erase(symbol);
			}
		}
	}

	assert(candidates.size() == 1);

	return *candidates.begin();
}

Grammar::RuleTable Grammar::Rules() {
	RuleTable table{};
	for (auto production : this->productions) {
		std::map<std::string, size_t> subTable{};
		for (size_t i = 0; i < production.second.size(); i++) {
			std::set<std::string> first = this->First(production.first, i);
			for (auto terminal : first) {
				if (subTable.find(terminal) != subTable.end()) {
					throw std::exception("duplicate terminal");
				}

				subTable.insert(std::make_pair(terminal, i));
			}
		}

		table.push_back(subTable);
	}

	return table;
}

std::set<std::string> Grammar::First(std::string symbol) {
	auto productions = this->productions.find(symbol);
	if (productions == this->productions.end()) {
		return { symbol };
	}

	std::set<std::string> first{};
	for (auto production : productions->second) {
		for (auto symbol : production) {
			std::set<std::string> tail = this->First(symbol);
			if (tail.size() != 0) {
				first.insert(tail.begin(), tail.end());
				break;
			}
		}
	}

	return first;
}

std::set<std::string> Grammar::First(std::string symbol, size_t index) {
	auto productions = this->productions.find(symbol)->second;
	auto production = productions[index];

	std::set<std::string> first{};
	for (auto symbol : production) {
		std::set<std::string> tail = this->First(symbol);
		if (tail.size() != 0) {
			first.insert(tail.begin(), tail.end());
			break;
		}
	}

	return first;
}
