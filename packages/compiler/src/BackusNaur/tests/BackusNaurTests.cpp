#include <gtest/gtest.h>
#include "BackusNaur/Parser.h"

using namespace BackusNaur;

TEST(Compiler, BNFTokenizerBasic) {
	Tokenizer tokenizer(R"(<expr> ::= <expr> ".")");

	EXPECT_EQ(tokenizer.Next(), Token(TokenType::Symbol, "expr"));
	EXPECT_EQ(tokenizer.Next(), Token(TokenType::Replaces, "::="));
	EXPECT_EQ(tokenizer.Next(), Token(TokenType::Symbol, "expr"));
	EXPECT_EQ(tokenizer.Next(), Token(TokenType::Literal, "."));
}

TEST(Compiler, BNFParserBasic) {
	Parser parser(
	R"(<expr> ::= <expr> ".";)");

	Grammar grammar = parser.Parse();

	EXPECT_EQ(grammar.size(), 1);
	EXPECT_EQ(grammar[0].symbol.name, "expr");
	EXPECT_EQ(grammar[0].expression.size(), 1);
	EXPECT_EQ(grammar[0].expression[0].size(), 2);
	EXPECT_EQ(std::get<Symbol>(grammar[0].expression[0][0]).name, "expr");
}
