#include "Grammar.h"

Grammar Grammar::Create(std::string source) {
	Grammar grammar{};
	grammar.ast = Parser::Parse(source);
	for (auto production : grammar.ast) {
		grammar.productions.insert(production);
	}

	return grammar;
}

std::set<std::string> Grammar::Terminals() {
	std::set<std::string> terminals{};

	for (auto production : this->ast) {
		for (auto sequence : production.second.first) {
			for (auto symbol : sequence) {
				if (this->productions.find(symbol) == this->productions.end()) {
					terminals.insert(symbol);
				}
			}
		}
	}

	return terminals;
}

std::map<size_t, std::set<std::string>> Grammar::Rules(std::string symbol) {
	std::map<std::string, size_t> table{};

	auto production = this->productions.at(symbol);
	for (size_t i = 0; i < production.first.size(); i++) {
		for (auto terminal : this->First(symbol, i)) {
			if (table.find(terminal) != table.end()) {
				throw std::exception("duplicate terminal");
			}

			table.insert(std::make_pair(terminal, i));
		}
	}

	std::map<size_t, std::set<std::string>> rules{};
	for (auto rule : table) {
		if (rules.find(rule.second) == rules.end()) {
			rules.insert(std::make_pair(rule.second, std::set<std::string>()));
		}

		rules.at(rule.second).insert(rule.first);
	}

	return rules;
}

std::set<std::string> Grammar::First(std::string symbol) {
	auto productions = this->productions.find(symbol);
	if (productions == this->productions.end()) {
		return { symbol };
	}

	std::set<std::string> first{};
	for (auto production : productions->second.first) {
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
	auto productions = this->productions.at(symbol).first;
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
