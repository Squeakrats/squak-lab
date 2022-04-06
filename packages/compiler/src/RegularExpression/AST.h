#pragma once
#include <string>
#include <memory>

namespace RegularExpression::AST {

struct RegularExpression;
struct Sequence;
struct Expression;
struct Quantifier;

struct RegularExpression {
	std::shared_ptr<Sequence> sequence;

	RegularExpression(Sequence* sequence) : sequence(sequence) {};
};

struct Sequence {
	std::shared_ptr<Expression> lhs;
	std::shared_ptr<Sequence> rhs;

	Sequence(Expression* lhs, Sequence* rhs) : lhs(lhs), rhs(rhs) {};
};

struct Expression {
	std::string character;
	std::shared_ptr<Quantifier> quantifier;

	Expression(std::string character, Quantifier* quantifier) : character(character), quantifier(quantifier) {};
};

struct Quantifier {
	std::string quantifier;

	Quantifier(std::string) : quantifier(quantifier) {};
};

}