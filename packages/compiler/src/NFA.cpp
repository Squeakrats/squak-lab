#include <assert.h>
#include "NFA.h"

static uint32_t nextStateId = 0;

uint32_t NFA::AddState() {
	this->states.insert(std::make_pair(nextStateId, NFAState()));

	return nextStateId++;
}

NFA::NFAState& NFA::GetState(uint32_t id) {
	auto state = this->states.find(id);
	assert(state != this->states.end());

	return state->second;
}

void NFA::AddTransition(uint32_t current, uint32_t next, char transition) {
	GetState(current).insert(std::make_pair(transition, next));
}

void NFA::Append(NFA&& b) {
	this->states.insert(b.states.begin(), b.states.end());
	this->AddTransition(this->acceptingState, b.initialState);
	this->acceptingState = b.acceptingState;
}

void NFA::Union(NFA&& b) {
	this->states.insert(b.states.begin(), b.states.end());

	uint32_t initialState = this->AddState();
	uint32_t acceptingState = this->AddState();

	this->AddTransition(initialState, this->initialState);
	this->AddTransition(initialState, b.initialState);
	this->AddTransition(this->acceptingState, acceptingState);
	this->AddTransition(b.acceptingState, acceptingState);

	this->initialState = initialState;
	this->acceptingState = acceptingState;
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
