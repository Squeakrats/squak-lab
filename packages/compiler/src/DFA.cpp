#include "DFA.h"

template<typename T>
class IDGenerator {
public:
	std::map<T, uint32_t> table{};

	uint32_t GetId(const T& value) {
		auto existingId = this->table.find(value);
		if (existingId != this->table.end()) {
			return existingId->second;
		}

		uint32_t newId = static_cast<uint32_t>(this->table.size());
		this->table.insert(std::make_pair(value, newId));

		return newId;
	}
};

DFA DFA::FromNFA(NFA nfa) {
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
				edges.at(edge.first).insert(epsilonClosure.begin(), epsilonClosure.end());
			}
		}

		states.insert(std::make_pair(current, edges));

		for (auto edge : edges) {
			if (states.find(edge.second) == states.end()) {
				work.insert(edge.second);
			}
		}
	}

	DFA dfa{};
	IDGenerator<std::set<uint32_t>> idGenerator{};
	dfa.initialState = idGenerator.GetId(initialState);

	for (auto state : states) {
		uint32_t id = idGenerator.GetId(state.first);
		std::map<char, uint32_t> edges;

		for (auto edge : state.second) {
			edges.insert(std::make_pair(edge.first, idGenerator.GetId(edge.second)));
		}

		uint32_t tag = 0;
		for (uint32_t stateId : state.first) {
			uint32_t stateTag = nfa.GetState(stateId).tag;
			if (stateTag != 0) {
				tag = stateTag;
				break;
			}
		}

		dfa.states.insert(std::make_pair(id, DFAState{ edges, tag }));
		if (state.first.find(nfa.acceptingState) != state.first.end()) {
			dfa.acceptingStates.insert(id);
		}
	}

	return dfa;
}

bool DFA::Match(std::string text) {
	uint32_t stateId = this->initialState;
	for (char character : text) {
		auto state = this->states.at(stateId);
		auto nextStateId = state.edges.find(character);
		if (nextStateId == state.edges.end()) {
			return false;
		}

		stateId = nextStateId->second;
	}

	return this->acceptingStates.find(stateId) != this->acceptingStates.end();
}

std::pair<std::string, uint32_t> DFA::Longest(std::stringstream& stream) {
	uint32_t stateId = this->initialState;
	std::pair<std::string, uint32_t> longest = std::make_pair("", 0);

	bool matched = false;

	while (!stream.eof()) {
		char character = stream.get();
		auto state = this->states.at(stateId);
		auto nextStateId = state.edges.find(character);
		if (nextStateId == state.edges.end()) {
			stream.putback(character);

			return longest;
		}

		longest.first += character;
		stateId = nextStateId->second;

		if (this->acceptingStates.find(stateId) != this->acceptingStates.end()) {
			matched = true;
			longest.second = this->states.at(stateId).tag;
		}
		else if (matched) {
			stream.putback(character);

			return longest;
		}
	}

	return longest;
}