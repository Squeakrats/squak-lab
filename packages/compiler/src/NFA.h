#pragma once
#include <map>
#include <set>
#include <string>

class NFA {
	struct NFAState {
		std::map<char, uint32_t> edges{};
		std::set<uint32_t> epsilons{};
		uint32_t tag{};
	};

public:
	std::map<uint32_t, NFAState> states{};
	uint32_t initialState{};
	uint32_t acceptingState{};

	uint32_t AddState();
	NFAState& GetState(uint32_t);

	void AddTransition(uint32_t current, uint32_t next, char transition);
	void AddTransition(uint32_t current, uint32_t next);
	void Append(NFA&& b);
	void Union(NFA&& b);

	std::set<uint32_t> calculateEpsilonClosure(uint32_t state);
};
