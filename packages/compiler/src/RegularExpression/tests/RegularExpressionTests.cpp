#include <gtest/gtest.h>
#include "RegularExpression/Tokenizer.h"

using namespace RegularExpression;

TEST(Compiler, RegularExpressionTokenizer) {
	std::stringstream source("aa+bb");

	EXPECT_EQ(Tokenize(source), Token(TokenType::Character, "a"));
	EXPECT_EQ(Tokenize(source), Token(TokenType::Character, "a"));
	EXPECT_EQ(Tokenize(source), Token(TokenType::Quantifier, "+"));
	EXPECT_EQ(Tokenize(source), Token(TokenType::Character, "b"));
	EXPECT_EQ(Tokenize(source), Token(TokenType::Character, "b"));
}
