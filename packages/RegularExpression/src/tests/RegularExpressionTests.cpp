#include <gtest/gtest.h>
#include  "../RegularExpression.h"

using namespace RegularExpression;

TEST(RegularExpression, PlusOperator) {
	DFA dfa = DFA::FromNFA(RegularExpression::Create("a+"));

	EXPECT_EQ(dfa.Match("a"), true);
	EXPECT_EQ(dfa.Match("aa"), true);
	EXPECT_EQ(dfa.Match("aaa"), true);
	EXPECT_EQ(dfa.Match("aaab"), false);
	EXPECT_EQ(dfa.Match(""), false);
}

TEST(RegularExpression, StarOperator) {
	DFA dfa = DFA::FromNFA(RegularExpression::Create("ba*"));

	EXPECT_EQ(dfa.Match("b"), true);
	EXPECT_EQ(dfa.Match("ba"), true);
	EXPECT_EQ(dfa.Match("baa"), true);
	EXPECT_EQ(dfa.Match("bb"), false);
	EXPECT_EQ(dfa.Match(""), false);
}

TEST(RegularExpression, OptionalOperator) {
	DFA dfa = DFA::FromNFA(RegularExpression::Create("a?c"));

	EXPECT_EQ(dfa.Match("ac"), true);
	EXPECT_EQ(dfa.Match("c"), true);
	EXPECT_EQ(dfa.Match("bc"), false);
	EXPECT_EQ(dfa.Match("ad"), false);
}

TEST(RegularExpression, CharacterClass) {
	DFA dfa = DFA::FromNFA(RegularExpression::Create("[abc]"));

	EXPECT_EQ(dfa.Match("a"), true);
	EXPECT_EQ(dfa.Match("b"), true);
	EXPECT_EQ(dfa.Match("c"), true);
	EXPECT_EQ(dfa.Match("d"), false);
}

TEST(RegularExpression, Number) {
	DFA dfa = DFA::FromNFA(RegularExpression::Create("[0123456789]+.?[0123456789]+"));

	EXPECT_EQ(dfa.Match("0123"), true);
	EXPECT_EQ(dfa.Match("434"), true);
	EXPECT_EQ(dfa.Match("433.232"), true);
	EXPECT_EQ(dfa.Match("32"), true);
	EXPECT_EQ(dfa.Match("32.a"), false);
}

TEST(RegularExpression, Negate) {
	DFA dfa = DFA::FromNFA(RegularExpression::Create("[^abc]"));

	EXPECT_EQ(dfa.Match("d"), true);
	EXPECT_EQ(dfa.Match("e"), true);
	EXPECT_EQ(dfa.Match("f"), true);
	EXPECT_EQ(dfa.Match("a"), false);
	EXPECT_EQ(dfa.Match("b"), false);
	EXPECT_EQ(dfa.Match("c"), false);
}

TEST(RegularExpression, Merge) {
	DFA dfa = DFA::FromNFA(RegularExpression::Create(std::vector<std::string>{
		"true",
		"false"
	}));

	std::stringstream source("truefalse");
	EXPECT_EQ(dfa.Longest(source).second, 1);
	EXPECT_EQ(dfa.Longest(source).second, 2);
}