#include <gtest/gtest.h>
#include <DFA.h>
#include <BNFParser.h>

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
	BNFTokenizer tokenizer(R"(<expr> ::= <expr> ".")");

	EXPECT_EQ(tokenizer.Next().type, BNFTokenizer::TokenType::Symbol);
	EXPECT_EQ(tokenizer.Next().type, BNFTokenizer::TokenType::Replaces);
	EXPECT_EQ(tokenizer.Next().type, BNFTokenizer::TokenType::Symbol);
	EXPECT_EQ(tokenizer.Next().type, BNFTokenizer::TokenType::Literal);
}

TEST(Compiler, BNFParserBasic) {
	BNFParser parser(
	R"(<expr> ::= <expr> ".";)");

	Grammar grammar = parser.Parse();

	EXPECT_EQ(grammar.size(), 1);
	EXPECT_EQ(grammar[0].symbol.name, "expr");
	EXPECT_EQ(grammar[0].expression.size(), 1);
	EXPECT_EQ(grammar[0].expression[0].size(), 2);
	EXPECT_EQ(std::get<Symbol>(grammar[0].expression[0][0]).name, "expr");
}
