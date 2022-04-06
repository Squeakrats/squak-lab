#include "RegularExpression.h"

namespace RegularExpression {

DFA Create(std::string expression) {
	NFA nfa;

	nfa.initialState = nfa.AddState();

	uint32_t currentState = nfa.initialState;
	for (char character : expression) {
		uint32_t nextState = nfa.AddState();
		nfa.AddTransition(currentState, nextState, character);
		currentState = nextState;
	}

	nfa.acceptingState = currentState;

	return DFA::FromNFA(nfa);
}

};