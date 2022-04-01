#include <gtest/gtest.h>
#include "../Grammar.h"

using namespace BackusNaur;

TEST(Compiler, BackusNaurGrammar) {
	Grammar grammar = Grammar::Create("<expr> ::= <literal>;");

	auto productions = grammar.productions.find("expr")->second;
	EXPECT_EQ(productions[0][0], "literal");
	EXPECT_EQ(grammar.GetTerminals(), std::set<std::string>({ "literal" }));
}