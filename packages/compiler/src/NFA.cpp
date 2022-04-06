#include <assert.h>
#include "NFA.h"


uint32_t NFA::AddState() {
	// TODO - not thread safe
	static uint32_t nextStateId = 1;
	this->states.insert(std::make_pair(nextStateId, NFAState()));

	return nextStateId++;
}

NFA::NFAState& NFA::GetState(uint32_t id) {
	auto state = this->states.find(id);
	assert(state != this->states.end());

	return state->second;
}

void NFA::AddTransition(uint32_t current, uint32_t next, char transition) {
	GetState(current).edges.insert(std::make_pair(transition, next));
}

void NFA::AddTransition(uint32_t current, uint32_t next) {
	GetState(current).epsilons.insert(next);
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

std::set<uint32_t> NFA::calculateEpsilonClosure(uint32_t state) {
	std::set<uint32_t> closure{ state };
	std::set<uint32_t> work{ state };

	while (!work.empty()) {
		uint32_t current = *work.begin();
		work.erase(current);

		for (uint32_t epsilon : this->GetState(current).epsilons) {
			if (closure.find(epsilon) == closure.end()) {
				closure.insert(epsilon);
				work.insert(epsilon);
			}
		}
	}

	return closure;
}
