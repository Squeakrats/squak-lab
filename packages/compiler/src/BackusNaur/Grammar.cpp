#include "Grammar.h"

namespace BackusNaur {

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

}