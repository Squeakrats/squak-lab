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
	if (ast.quantifier == nullptr) {
		return Create(ast.character[0]);
	}

	if (ast.quantifier->quantifier == "+") {
		NFA start = Create(ast.character[0]);
		NFA repeat = Create(ast.character[0]);

		start.Union(std::move(repeat));
		start.AddTransition(start.acceptingState, repeat.initialState);

		return start;
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