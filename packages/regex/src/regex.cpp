#include "regex.h"
#include "Parser.generated.h"
#include "utility.h"

namespace regex {

NFA Create(char character) {
  NFA nfa;
  nfa.initialState = nfa.AddState();
  nfa.acceptingState = nfa.AddState();
  nfa.AddTransition(nfa.initialState, nfa.acceptingState, character);

  return nfa;
}

NFA Create(ast::ValueNode& valueNode) {
  auto& value = valueNode.value;

  if (std::holds_alternative<char>(value)) {
    return Create(std::get<char>(value));
  }

  if (std::holds_alternative<std::unique_ptr<ast::CharacterClassNode>>(value)) {
    ast::CharacterClassNode& characterClassNode =
      *std::get<std::unique_ptr<ast::CharacterClassNode>>(value);

    bool characters[256] = { 0 };

    for (char character : characterClassNode.list) {
      characters[static_cast<uint8_t>(character)] = true;
    }

    if (characterClassNode.negated) {
      for (int i = 0; i < 256; i++) {
        characters[i] = !characters[i];
      }
    }

    NFA nfa{};
    nfa.initialState = nfa.AddState();
    nfa.acceptingState = nfa.AddState();
    for (int i = 0; i < 256; i++) {
      if (characters[i]) {
        nfa.AddTransition(nfa.initialState, nfa.acceptingState, i);
      }
      characters[i] = !characters[i];
    }

    return nfa;
  }

  Panic("unhandled value");
}

NFA Create(ast::ExpressionNode& ast) {
  NFA nfa = Create(*ast.value);

  if (ast.quantifier == nullptr) {
    return nfa;
  }

  char quantifier = ast.quantifier->quantifier[0];

  if (quantifier == '+') {
    NFA repeat = Create(*ast.value);

    nfa.Union(std::move(repeat));
    nfa.AddTransition(nfa.acceptingState, repeat.initialState);

    return nfa;
  }

  if (quantifier == '*') {
    nfa.AddTransition(nfa.initialState, nfa.acceptingState);
    nfa.AddTransition(nfa.acceptingState, nfa.initialState);

    return nfa;
  }

  if (quantifier == '?') {
    nfa.AddTransition(nfa.initialState, nfa.acceptingState);

    return nfa;
  }

  Panic("unhandled quantifier");
}

NFA Create(ast::RegularExpressionNode& ast) {
  NFA nfa;
  nfa.initialState = nfa.AddState();
  nfa.acceptingState = nfa.initialState;

  for (auto& expression : ast.expressions) {
    nfa.Append(Create(*expression));
  }

  return nfa;
}

NFA CreateNFA(std::string expression) {
  ParserContext context{expression, Tokenize};
  ast::RegularExpressionNode* ast =
    static_cast<ast::RegularExpressionNode*>(Parseregex(context));
  NFA nfa = Create(*ast);
  delete ast;

  return nfa;
}

DFA Create(std::string expression) {
  return DFA::FromNFA(CreateNFA(expression));
}

DFA Create(std::vector<std::string> expressions) {
  NFA result{};
  result.initialState = result.AddState();
  result.acceptingState = result.AddState();

  for (uint32_t i = 0; i < expressions.size(); i++) {
    NFA nfa = CreateNFA(expressions[i]);
    nfa.GetState(nfa.acceptingState).tag = i + 1;

    result.Union(std::move(nfa));
  }

  return DFA::FromNFA(result);
}

};