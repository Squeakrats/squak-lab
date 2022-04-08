#include <gtest/gtest.h>
#include  "../parser.h"

using namespace JSON;

TEST(JSON, Tokenize) {
	std::stringstream stream("{}");

	EXPECT_EQ(Tokenize(stream).first, TokenType::EndOfFile);
}