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

NFA Create(AST::Value& value) {
  if (value.character != nullptr) {
    return Create(value.character->value[0]);
  }

  if (value.characterClass != nullptr &&
      value.characterClass->list != nullptr) {
    bool characters[256] = { 0 };

    std::shared_ptr<AST::CharacterClassList> current =
      value.characterClass->list;
    while (current != nullptr) {
      characters[static_cast<uint8_t>(current->character[0])] = true;
      current = current->rhs;
    }

    if (value.characterClass->negated) {
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

NFA Create(AST::Expression& ast) {
  NFA nfa = Create(*ast.value.get());

  if (ast.quantifier == nullptr) {
    return nfa;
  }

  if (ast.quantifier->quantifier == "+") {
    NFA repeat = Create(*ast.value.get());

    nfa.Union(std::move(repeat));
    nfa.AddTransition(nfa.acceptingState, repeat.initialState);

    return nfa;
  }

  if (ast.quantifier->quantifier == "*") {
    nfa.AddTransition(nfa.initialState, nfa.acceptingState);
    nfa.AddTransition(nfa.acceptingState, nfa.initialState);

    return nfa;
  }

  if (ast.quantifier->quantifier == "?") {
    nfa.AddTransition(nfa.initialState, nfa.acceptingState);

    return nfa;
  }

  Panic("unhandled quantifier");
}

NFA Create(AST::RegularExpression& ast) {
  NFA nfa;
  nfa.initialState = nfa.AddState();
  nfa.acceptingState = nfa.initialState;

  std::shared_ptr<AST::Sequence> current = ast.sequence;
  while (current != nullptr) {
    if (current->lhs != nullptr) {
      nfa.Append(Create(*current->lhs));
    }

    current = current->rhs;
  }

  return nfa;
}

NFA Create(std::string expression) {
  TokenStream<Token> stream(expression, Tokenize);
  ParserContext context{ stream.Next(), stream };
  NFA nfa = Create(*Parseregex(context));

  return nfa;
}

NFA Create(std::vector<std::string> expressions) {
  NFA result{};
  result.initialState = result.AddState();
  result.acceptingState = result.AddState();

  for (uint32_t i = 0; i < expressions.size(); i++) {
    NFA nfa = Create(expressions[i]);
    nfa.GetState(nfa.acceptingState).tag = i + 1;

    result.Union(std::move(nfa));
  }

  return result;
}

};