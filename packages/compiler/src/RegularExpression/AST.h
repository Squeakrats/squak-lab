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
	std::shared_ptr<Value> value;
	std::shared_ptr<Quantifier> quantifier;

	Expression(Value* value, Quantifier* quantifier) : value(value), quantifier(quantifier) {};
};

struct Character {
	std::string value;

	Character(std::string value) : value(value) {};
};


struct CharacterClass {
	std::shared_ptr<CharacterClassList> list;

	CharacterClass(CharacterClassList* list) : list(list) {};
};

struct CharacterClassList {
	std::string character;
	std::shared_ptr<CharacterClassList> rhs;

	CharacterClassList(std::string character, CharacterClassList* rhs) : character(character), rhs(rhs) {};
};

struct Value {
	std::shared_ptr<Character> character;
	std::shared_ptr<CharacterClass> characterClass;

	Value(Character* character) : character(character) {};
	Value(CharacterClass* characterClass) : characterClass(characterClass) {};
};

struct Quantifier {
	std::string quantifier;

	Quantifier(std::string quantifier) : quantifier(quantifier) {};
};

}