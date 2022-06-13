#include "DFA.h"

class IDGenerator {
public:
  std::map<std::set<uint32_t>, uint32_t> table{};

  uint32_t GetId(const std::set<uint32_t> value) {
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

  std::set<uint32_t> initialState =
    nfa.calculateEpsilonClosure(nfa.initialState);
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

        std::set<uint32_t> epsilonClosure =
          nfa.calculateEpsilonClosure(edge.second);
        edges.at(edge.first)
          .insert(epsilonClosure.begin(), epsilonClosure.end());
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
  IDGenerator idGenerator{};
  dfa.initialState = idGenerator.GetId(initialState);

  for (auto state : states) {
    uint32_t id = idGenerator.GetId(state.first);

    uint32_t tag = 0;
    for (uint32_t stateId : state.first) {
      uint32_t stateTag = nfa.GetState(stateId).tag;
      if (tag == 0) {
        tag = stateTag;
      } else if (stateTag != 0) {
        tag = std::min(tag, stateTag);
      }
    }

    DFAState& dfaState = dfa.AddState(id, tag);
    
    for (auto edge : state.second) {
      dfaState.edges[static_cast<uint8_t>(edge.first)] =
        std::make_pair(true, idGenerator.GetId(edge.second));
    }

    if (state.first.find(nfa.acceptingState) != state.first.end()) {
      dfa.acceptingStates.insert(id);
    }
  }

  return dfa;
}

bool DFA::Match(std::string text) {
  uint32_t stateId = this->initialState;
  for (char character : text) {
    uint8_t hash = static_cast<uint8_t>(character);
    auto state = this->states.at(stateId);
    if (!state.edges[hash].first) {
      return false;
    }

    stateId = state.edges[hash].second;
  }

  return this->acceptingStates.find(stateId) != this->acceptingStates.end();
}

std::pair<std::string, uint32_t> DFA::Longest(IByteStream& stream) {
  uint32_t stateId = this->initialState;
  size_t initialPosition = stream.GetPosition();

  size_t pending = 0;

  std::string text{};
  size_t length = 0;
  uint32_t tag = 0;

  while (!stream.AtEnd()) {
    char character = stream.Peek();
    uint8_t hash = static_cast<uint8_t>(character);

    auto& state = this->states.at(stateId);
    if (!state.edges[hash].first) {
      stream.SetPosition(initialPosition);
      text.resize(length);
      stream.Read(text.data(), length);

      return std::make_pair(text, tag);
    }

    stream.Get();
    pending++;

    stateId = state.edges[hash].second;

    if (this->acceptingStates.find(stateId) != this->acceptingStates.end()) {
      tag = this->states.at(stateId).tag;
      length = pending;
    }
  }

  stream.SetPosition(initialPosition);
  text.resize(length);
  stream.Read(text.data(), length);

  return std::make_pair(text, tag);
}