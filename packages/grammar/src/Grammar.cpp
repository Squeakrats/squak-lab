#include "Grammar.h"

Grammar Grammar::Create(std::string source) {
  Grammar grammar{};
  grammar.ast = Parser::Parse(source);
  for (auto production : grammar.ast.productions) {
    grammar.productions.insert(std::make_pair(production.symbol, production));
  }

  return grammar;
}

std::set<std::string> Grammar::Terminals() {
  std::set<std::string> terminals{};

  for (auto production : this->ast.productions) {
    for (auto sequence : production.expression) {
      for (auto symbol : sequence.symbols) {
        if (this->productions.find(symbol) == this->productions.end()) {
          terminals.insert(symbol);
        }
      }
    }
  }

  return terminals;
}

std::map<size_t, std::set<std::string>> Grammar::Rules(std::string symbol) {
  std::map<std::string, size_t> table{};

  auto production = this->productions.at(symbol);
  for (size_t i = 0; i < production.expression.size(); i++) {
    for (auto terminal : this->First(symbol, i)) {
      if (table.find(terminal) != table.end()) {
        Panic("duplicate terminal");
      }

      table.insert(std::make_pair(terminal, i));
    }
  }

  std::map<size_t, std::set<std::string>> rules{};
  for (auto rule : table) {
    if (rules.find(rule.second) == rules.end()) {
      rules.insert(std::make_pair(rule.second, std::set<std::string>()));
    }

    rules.at(rule.second).insert(rule.first);
  }

  return rules;
}

std::set<std::string> Grammar::First(std::string symbol) {
  auto productions = this->productions.find(symbol);
  if (productions == this->productions.end()) {
    return { symbol };
  }

  std::set<std::string> first{};
  for (auto production : productions->second.expression) {
    for (auto symbol : production.symbols) {
      std::set<std::string> tail = this->First(symbol);
      if (tail.size() != 0) {
        first.insert(tail.begin(), tail.end());
        break;
      }
    }
  }

  return first;
}

std::set<std::string> Grammar::First(std::string symbol, size_t index) {
  auto productions = this->productions.at(symbol).expression;
  auto production = productions[index];

  std::set<std::string> first{};
  for (auto symbol : production.symbols) {
    std::set<std::string> tail = this->First(symbol);
    if (tail.size() != 0) {
      first.insert(tail.begin(), tail.end());
      break;
    }
  }

  return first;
}
