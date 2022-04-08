#include <gtest/gtest.h>
#include  "../parser.h"

using namespace JSON;

TEST(JSON, Tokenize) {
	std::stringstream stream(R"({"Key":"Value"})");
	EXPECT_EQ(Tokenize(stream).first, TokenType::LeftBrace);
	EXPECT_EQ(Tokenize(stream).first, TokenType::StringLiteral);
	EXPECT_EQ(Tokenize(stream).first, TokenType::Colon);
	EXPECT_EQ(Tokenize(stream).first, TokenType::StringLiteral);
	EXPECT_EQ(Tokenize(stream).first, TokenType::RightBrace);
	EXPECT_EQ(Tokenize(stream).first, TokenType::EndOfFile);
}