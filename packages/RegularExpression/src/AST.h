#pragma once
#include <string>
#include <memory>

namespace RegularExpression::AST {

struct RegularExpression;
struct Sequence;
struct Expression;
struct Quantifier;
struct Value;
struct CharacterClassList;
struct Not;

struct RegularExpression {
	std::shared_ptr<Sequence> sequence;

	RegularExpression(std::shared_ptr<Sequence> sequence) : sequence(sequence) {};
};

struct Sequence {
	std::shared_ptr<Expression> lhs;
	std::shared_ptr<Sequence> rhs;

	Sequence(std::shared_ptr<Expression> lhs, std::shared_ptr<Sequence> rhs) : lhs(lhs), rhs(rhs) {};
};

struct Expression {
	std::shared_ptr<Value> value;
	std::shared_ptr<Quantifier> quantifier;

	Expression(std::shared_ptr<Value> value, std::shared_ptr<Quantifier> quantifier) : value(value), quantifier(quantifier) {};
};

struct Character {
	std::string value;

	Character(std::string value) : value(value) {};
};


struct CharacterClass {
	std::shared_ptr<Not> not;
	std::shared_ptr<CharacterClassList> list;

	CharacterClass(std::shared_ptr<Not> not, std::shared_ptr<CharacterClassList> list) : not(not), list(list) {};
};

struct CharacterClassList {
	std::string character;
	std::shared_ptr<CharacterClassList> rhs;

	CharacterClassList(std::string character, std::shared_ptr<CharacterClassList> rhs) : character(character), rhs(rhs) {};
};

struct Value {
	std::shared_ptr<Character> character;
	std::shared_ptr<CharacterClass> characterClass;

	Value(std::shared_ptr<Character> character) : character(character) {};
	Value(std::shared_ptr<CharacterClass> characterClass) : characterClass(characterClass) {};
};

struct Not {};

struct Quantifier {
	std::string quantifier;

	Quantifier(std::string quantifier) : quantifier(quantifier) {};
};

}