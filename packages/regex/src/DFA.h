#pragma once
#include "NFA.h"
#include <sstream>
#include <array>

class DFA {
  struct DFAState {
    std::array<std::pair<bool, uint32_t>, 256> edges{};
    uint32_t tag{};
  };

public:
  std::map<uint32_t, DFAState> states{};
  uint32_t initialState{};
  std::set<uint32_t> acceptingStates{};

  DFAState& AddState(uint32_t id, uint32_t tag) {
    this->states.insert(std::make_pair(id, DFAState{}));
    DFAState& state = this->states.at(id);
    state.tag = tag;

    return state;
  }

  bool Match(std::string);
  std::pair<std::string, uint32_t> Longest(std::stringstream& stream);

  static DFA FromNFA(NFA nfa);
};
