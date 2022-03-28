#include "DFA.h"

DFA DFA::FromRegularExpression(std::string expression) {
	return DFA::FromNFA(NFA::FromRegularExpression(expression));
}

DFA DFA::FromNFA(NFA& nfa) {
	std::map<std::set<uint32_t>, std::map<char, std::set<uint32_t>>> states{};

	std::set<uint32_t> initialState = nfa.calculateEpsilonClosure(nfa.initialState);
	std::set<std::set<uint32_t>> work{ initialState };

	while (!work.empty()) {
		std::set<uint32_t> current = *work.begin();
		work.erase(current);

		std::map<char, std::set<uint32_t>> edges;
		for (auto state : current) {
			for (auto edge : nfa.GetState(state).edges) {
				if (edges.find(edge.first) == edges.end()) {
					edges.insert(std::make_pair(edge.first, std::set<uint32_t>()));
				}

				std::set<uint32_t> epsilonClosure = nfa.calculateEpsilonClosure(edge.second);
				edges.find(edge.first)->second.insert(epsilonClosure.begin(), epsilonClosure.end());
			}
		}

		states.insert(std::make_pair(current, edges));
		for (auto edge : edges) {
			if (states.find(edge.second) == states.end()) {
				work.insert(edge.second);
			}
		}
	}

	auto ensureId = [](const std::set<uint32_t>& id) {
		static std::map<std::set<uint32_t>, uint32_t> idTable{};

		auto existingId = idTable.find(id);
		if (existingId != idTable.end()) {
			return existingId->second;
		}

		uint32_t newId = static_cast<uint32_t>(idTable.size());
		idTable.insert(std::make_pair(id, newId));

		return newId;
	};

	DFA dfa{};
	dfa.initialState = ensureId(initialState);

	for (auto state : states) {
		uint32_t id = ensureId(state.first);
		std::map<char, uint32_t> edges;

		for (auto edge : state.second) {
			edges.insert(std::make_pair(edge.first, ensureId(edge.second)));
		}

		dfa.states.insert(std::make_pair(id, edges));
		if (state.first.find(nfa.acceptingState) != state.first.end()) {
			dfa.acceptingStates.insert(id);
		}
	}

	return dfa;
}

bool DFA::Match(std::string text) {
	uint32_t state = this->initialState;
	for (char character : text) {
		auto edges = this->states.find(state)->second;
		auto nextState = edges.find(character);
		if (nextState == edges.end()) {
			return false;
		}

		state = nextState->second;
	}

	return this->acceptingStates.find(state) != this->acceptingStates.end();
}