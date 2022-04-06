#include <gtest/gtest.h>
#include  "RegularExpression/RegularExpression.h"

using namespace RegularExpression;

TEST(Compiler, RegularExpressionTokenizer) {
	DFA dfa = RegularExpression::Create("a+");

	EXPECT_EQ(dfa.Match("a"), true);
	EXPECT_EQ(dfa.Match("aa"), true);
	EXPECT_EQ(dfa.Match("aaa"), true);
	EXPECT_EQ(dfa.Match("aaab"), false);
	EXPECT_EQ(dfa.Match(""), false);
}
