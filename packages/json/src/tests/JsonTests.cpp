#include <gtest/gtest.h>
#include  "../parser.h"

using namespace JSON;

TEST(JSON, Parse) {
	std::string rawJSON(R"(
		{
			"Key" : "Value"
		}
	)");

	TokenStream<Token> stream(rawJSON, Tokenize);
	ParserContext context{ stream.Next(), stream };

	std::shared_ptr<AST::Object> object = ParseJSON(context);

	EXPECT_EQ(object->entries->entry->key, "\"Key\"");
}