#include <gtest/gtest.h>
#include "RegularExpression/Parser.h"

using namespace RegularExpression;

TEST(Compiler, RegularExpressionTokenizer) {
	std::string source("aa+bb");
	TokenStream<Token> stream(source, Tokenize);
	ParserContext context{ stream.Next(), stream };

	AST::RegularExpression* ast = ParseRegularExpression(context);
	ASSERT_EQ(ast->sequence->lhs->character, "a");
}
