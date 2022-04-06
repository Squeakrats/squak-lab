#include <gtest/gtest.h>
#include  "RegularExpression/RegularExpression.h"

using namespace RegularExpression;

TEST(Compiler, RegularExpressionTokenizer) {
	DFA dfa = RegularExpression::Create("abc");

	EXPECT_EQ(dfa.Match("abc"), true);
	EXPECT_EQ(dfa.Match("def"), false);
	EXPECT_EQ(dfa.Match("ghj"), false);
}
