#include "RegularExpression.h"
#include "Parser.h"

namespace RegularExpression {

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

	if (value.characterClass != nullptr && value.characterClass->list != nullptr) {
		bool characters[256] = { 0 };

		std::shared_ptr<AST::CharacterClassList> current = value.characterClass->list;
		while (current != nullptr) {
			characters[current->character[0]] = true;
			current = current->rhs;
		}

		if (value.characterClass->not != nullptr) {
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

	throw std::exception("unhandle value");
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

	throw std::exception("unhandle quantifier");
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

DFA Create(std::string expression) {
	TokenStream<Token> stream(expression, Tokenize);
	ParserContext context{ stream.Next(), stream };
	AST::RegularExpression* ast = ParseRegularExpression(context);
	NFA nfa = Create(*ast);

	return DFA::FromNFA(nfa);
}

};