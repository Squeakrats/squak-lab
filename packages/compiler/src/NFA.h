#pragma once
#include <map>
#include <set>
#include <string>

class NFA {
	using NFAState = std::multimap<char, uint32_t>;

public:
	std::map<uint32_t, NFAState> states{};
	uint32_t initialState{};
	uint32_t acceptingState{};

	uint32_t AddState();
	void AddTransition(uint32_t current, uint32_t next, char transition);
	void AddTransition(uint32_t current, uint32_t next);

	static NFA FromRegularExpression(std::string expression);
};
