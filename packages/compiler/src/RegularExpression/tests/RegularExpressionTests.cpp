#include <gtest/gtest.h>
#include "RegularExpression/Parser.h"
#include  "RegularExpression/RegularExpression.h"

using namespace RegularExpression;

TEST(Compiler, RegularExpressionTokenizer) {
	TokenStream<Token> stream("aa+bb", Tokenize);
	ParserContext context{ stream.Next(), stream };

	AST::RegularExpression* ast = ParseRegularExpression(context);
	ASSERT_EQ(ast->sequence->lhs->character, "a");

	DFA dfa = RegularExpression::Create("abc");

	EXPECT_EQ(dfa.Match("abc"), true);
	EXPECT_EQ(dfa.Match("def"), false);
	EXPECT_EQ(dfa.Match("ghj"), false);
}
