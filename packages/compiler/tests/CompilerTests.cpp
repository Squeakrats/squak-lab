#include <gtest/gtest.h>
#include "DFA.h"
#include "BackusNaur/Parser.h"
#include "RegularExpression/Tokenizer.h"

TEST(Compiler, NFABasic) {
	NFA nfa = NFA::FromRegularExpression("abc");
	nfa.Append(NFA::FromRegularExpression("def"));

	DFA dfa = DFA::FromNFA(nfa);
	EXPECT_EQ(dfa.Match("abcdef"), true);

	nfa.Union(NFA::FromRegularExpression("ghjklm"));
	dfa = DFA::FromNFA(nfa);

	EXPECT_EQ(dfa.Match("abcdef"), true);
	EXPECT_EQ(dfa.Match("ghjklm"), true);
	EXPECT_EQ(dfa.Match("abcghj"), false);
}

TEST(Compiler, BNFTokenizerBasic) {
	BackusNaur::Tokenizer tokenizer(R"(<expr> ::= <expr> ".")");

	EXPECT_EQ(tokenizer.Next().type, BackusNaur::TokenType::Symbol);
	EXPECT_EQ(tokenizer.Next().type, BackusNaur::TokenType::Replaces);
	EXPECT_EQ(tokenizer.Next().type, BackusNaur::TokenType::Symbol);
	EXPECT_EQ(tokenizer.Next().type, BackusNaur::TokenType::Literal);
}

TEST(Compiler, BNFParserBasic) {
	BackusNaur::Parser parser(
	R"(<expr> ::= <expr> ".";)");

	BackusNaur::Grammar grammar = parser.Parse();

	EXPECT_EQ(grammar.size(), 1);
	EXPECT_EQ(grammar[0].symbol.name, "expr");
	EXPECT_EQ(grammar[0].expression.size(), 1);
	EXPECT_EQ(grammar[0].expression[0].size(), 2);
	EXPECT_EQ(std::get<BackusNaur::Symbol>(grammar[0].expression[0][0]).name, "expr");
}

TEST(Compiler, RegularExpressionTokenizer) {
	RegularExpression::Tokenizer tokenizer("aa+bb");

	EXPECT_EQ(tokenizer.Next().text, "a");
	EXPECT_EQ(tokenizer.Next().text, "a");
	EXPECT_EQ(tokenizer.Next().type, RegularExpression::TokenType::Plus);
	EXPECT_EQ(tokenizer.Next().text, "b");
	EXPECT_EQ(tokenizer.Next().text, "b");
}