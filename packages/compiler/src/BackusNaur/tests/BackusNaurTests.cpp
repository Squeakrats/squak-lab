#include <gtest/gtest.h>
#include "BackusNaur/Parser.h"

using namespace BackusNaur;

TEST(Compiler, BNFTokenizerBasic) {
	Tokenizer tokenizer(R"(<expr> ::= <literal>;)");

	EXPECT_EQ(tokenizer.Next(), Token(TokenType::Symbol, "expr"));
	EXPECT_EQ(tokenizer.Next(), Token(TokenType::Replaces, "::="));
	EXPECT_EQ(tokenizer.Next(), Token(TokenType::Symbol, "literal"));
}

TEST(Compiler, BNFParserBasic) {
	Parser parser(
	R"(<expr> ::= <literal>;)");

	AST::Grammar grammar = parser.Parse();

	EXPECT_EQ(grammar.size(), 1);
	EXPECT_EQ(grammar[0].first, "expr");
	EXPECT_EQ(grammar[0].second.size(), 1);
	EXPECT_EQ(grammar[0].second[0].size(), 2);
	EXPECT_EQ(grammar[0].second[0][0], "literal");
}
