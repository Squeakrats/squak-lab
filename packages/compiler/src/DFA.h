#pragma once
#include "NFA.h"

class DFA {
	struct DFAState {
		std::map<char, uint32_t> edges{};
		uint32_t tag{};
	};

public:
	std::map<uint32_t, DFAState> states{};
	uint32_t initialState{};
	std::set<uint32_t> acceptingStates{};

	bool Match(std::string);
	std::pair<size_t, size_t> Longest(std::string);

	static DFA FromNFA(NFA& nfa);
};
