#include <gtest/gtest.h>
#include <format>
#include "../Grammar.h"

using namespace BackusNaur;

TEST(Compiler, BackusNaurGrammar) {
	Grammar grammar = Grammar::Create(
	"<Expression> ::= <Statement> <SemiColon> | <SemiColon>;"
	"<Statement> ::= <Literal> | <Symbol>;"
	);

	auto productions = grammar.productions.find("Expression")->second;
	EXPECT_EQ(productions[0][0], "Statement");
	EXPECT_EQ(grammar.Terminals(), std::set<std::string>({ "Literal", "SemiColon", "Symbol"}));
	EXPECT_EQ(grammar.First("Expression"), std::set<std::string>({ "Literal", "SemiColon", "Symbol"}));
	EXPECT_EQ(grammar.Rules().size(), 2);
}