#include <assert.h>
#include "NFA.h"

uint32_t NFA::AddState() {
	uint32_t nextId = static_cast<uint32_t>(this->states.size());
	this->states.insert(std::make_pair(nextId, NFAState()));

	return nextId;
}

void NFA::AddTransition(uint32_t current, uint32_t next, char transition) {
	auto state = this->states.find(current);
	assert(state != this->states.end());
	state->second.insert(std::make_pair(transition, next));
}

NFA NFA::FromRegularExpression(std::string expression) {
	NFA automata;

	automata.initialState = automata.AddState();

	uint32_t currentState = automata.initialState;
	for (char character : expression) {
		uint32_t nextState = automata.AddState();
		automata.AddTransition(currentState, nextState, character);
		currentState = nextState;
	}

	automata.acceptingState = currentState;

	return automata;
}
