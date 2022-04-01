#include <gtest/gtest.h>
#include "RegularExpression/Tokenizer.h"

using namespace RegularExpression;

TEST(Compiler, RegularExpressionTokenizer) {
	Tokenizer tokenizer("aa+bb");

	EXPECT_EQ(tokenizer.Next(), Token(TokenType::Character, "a"));
	EXPECT_EQ(tokenizer.Next(), Token(TokenType::Character, "a"));
	EXPECT_EQ(tokenizer.Next(), Token(TokenType::Quantifier, "+"));
	EXPECT_EQ(tokenizer.Next(), Token(TokenType::Character, "b"));
	EXPECT_EQ(tokenizer.Next(), Token(TokenType::Character, "b"));
}
