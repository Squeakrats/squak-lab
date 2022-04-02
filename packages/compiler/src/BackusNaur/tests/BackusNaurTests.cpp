#include <gtest/gtest.h>
#include "../Grammar.h"

using namespace BackusNaur;

TEST(Compiler, BackusNaurGrammar) {
	Grammar grammar = Grammar::Create(
	"<Expression> ::= <Literal> <SemiColon> | <SemiColon>;"
	);

	auto productions = grammar.productions.find("Expression")->second;
	EXPECT_EQ(productions[0][0], "Literal");
	EXPECT_EQ(grammar.Terminals(), std::set<std::string>({ "Literal", "SemiColon"}));
	EXPECT_EQ(grammar.First("Expression"), std::set<std::string>({ "Literal", "SemiColon"}));
}