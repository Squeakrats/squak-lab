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

NFA Create(AST::Expression& ast) {
	return Create(ast.character[0]);
}

NFA Create(AST::RegularExpression& ast) {
	NFA nfa;
	nfa.initialState = nfa.AddState();
	nfa.acceptingState = nfa.initialState;

	AST::Sequence* current = ast.sequence;
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