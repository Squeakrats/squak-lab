#include "Grammar.h"

namespace BackusNaur {

Grammar Grammar::Create(std::string source) {
	Grammar grammar{};
	for (auto production : Parser::Parse(source)) {
		grammar.productions.insert(production);
	}

	return grammar;
}

std::set<std::string> Grammar::GetTerminals() {
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

}