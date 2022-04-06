#pragma once
#include <string>

namespace RegularExpression::AST {

struct RegularExpression;
struct Sequence;
struct Expression;
struct Quantifier;

struct RegularExpression {
	Sequence* sequence;

	RegularExpression(Sequence* sequence) : sequence(sequence) {};
};

struct Sequence {
	Expression* lhs;
	Sequence* rhs;

	Sequence(Expression* lhs, Sequence* rhs) : lhs(lhs), rhs(rhs) {};
};

struct Expression {
	std::string character;
	Quantifier* quantifier;

	Expression(std::string character, Quantifier* quantifier) : character(character), quantifier(quantifier) {};
};

struct Quantifier {
	std::string quantifier;

	Quantifier(std::string) : quantifier(quantifier) {};
};

}