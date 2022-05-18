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
  if (std::holds_alternative<char>(value)) {
    return Create(std::get<char>(value));
  }

  if (std::holds_alternative<AST::CharacterClass>(value)) {
    AST::CharacterClass& characterClass = std::get<AST::CharacterClass>(value);

    bool characters[256] = { 0 };

    for (char character : characterClass.list) {
      characters[static_cast<uint8_t>(character)] = true;
    }

    if (characterClass.negated) {
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
  NFA nfa = Create(ast.value);

  if (ast.quantifier == std::nullopt) {
    return nfa;
  }

  char quantifier = *ast.quantifier;

  if (quantifier == '+') {
    NFA repeat = Create(ast.value);

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

NFA Create(AST::RegularExpression& ast) {
  NFA nfa;
  nfa.initialState = nfa.AddState();
  nfa.acceptingState = nfa.initialState;

  for (auto& expression : ast.sequence) {
    nfa.Append(Create(expression));
  }

  return nfa;
}

NFA CreateNFA(std::string expression) {
  TokenStream<Token> stream(expression, Tokenize);
  ParserContext context{ stream.Next(), stream };
  AST::RegularExpression ast = Parseregex(context);
  NFA nfa = Create(ast);

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

AST::Sequence AST::CreateSequence(Expression a, Sequence b) {
  std::vector<Expression> sequence{ a };
  sequence.insert(sequence.end(), b.begin(), b.end());

  return sequence;
}

};