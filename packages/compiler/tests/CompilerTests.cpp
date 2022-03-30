#include <gtest/gtest.h>
#include <DFA.h>
#include <BNF.h>

TEST(CompilerTests, NFATests) {
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

TEST(CompilerTests, BNFTests) {
	std::stringstream source(R"(<expr> ::= <expr> ".")");
	BNFTokenizer tokenizer(source);

	EXPECT_EQ(tokenizer.Next().type, BNFTokenizer::TokenType::Symbol);
	EXPECT_EQ(tokenizer.Next().type, BNFTokenizer::TokenType::Replaces);
	EXPECT_EQ(tokenizer.Next().type, BNFTokenizer::TokenType::Symbol);
	EXPECT_EQ(tokenizer.Next().type, BNFTokenizer::TokenType::Terminal);
}