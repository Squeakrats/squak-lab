#pragma once
#include "NFA.h"

class DFA {
	using DFAState = std::map<char, uint32_t>;

public:
	std::map<uint32_t, DFAState> states{};
	uint32_t initialState{};
	std::set<uint32_t> acceptingStates{};

	bool Match(std::string);
	size_t Longest(std::string);

	static DFA FromNFA(NFA& nfa);
};
