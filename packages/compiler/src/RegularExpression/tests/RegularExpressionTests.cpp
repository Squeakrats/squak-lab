#include <gtest/gtest.h>
#include  "RegularExpression/RegularExpression.h"

using namespace RegularExpression;

TEST(RegularExpression, PlusOperator) {
	DFA dfa = RegularExpression::Create("a+");

	EXPECT_EQ(dfa.Match("a"), true);
	EXPECT_EQ(dfa.Match("aa"), true);
	EXPECT_EQ(dfa.Match("aaa"), true);
	EXPECT_EQ(dfa.Match("aaab"), false);
	EXPECT_EQ(dfa.Match(""), false);
}

TEST(RegularExpression, OptionalOperator) {
	DFA dfa = RegularExpression::Create("a?c");

	EXPECT_EQ(dfa.Match("ac"), true);
	EXPECT_EQ(dfa.Match("c"), true);
	EXPECT_EQ(dfa.Match("bc"), false);
	EXPECT_EQ(dfa.Match("ad"), false);
}

TEST(RegularExpression, CharacterClass) {
	DFA dfa = RegularExpression::Create("[abc]");

	EXPECT_EQ(dfa.Match("a"), true);
	EXPECT_EQ(dfa.Match("b"), true);
	EXPECT_EQ(dfa.Match("c"), true);
	EXPECT_EQ(dfa.Match("d"), false);
}