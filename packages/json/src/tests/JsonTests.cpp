#include <gtest/gtest.h>
#include  "../parser.h"

using namespace JSON;

TEST(JSON, Parse) {
	std::string rawJSON(R"(
		{
			"Key1" : "Value1",
			"Key2" :  true
		}
	)");

	TokenStream<Token> stream(rawJSON, Tokenize);
	ParserContext context{ stream.Next(), stream };

	std::shared_ptr<AST::Object> object = ParseJSON(context);

	EXPECT_EQ(object->entries->entry->key, "\"Key1\"");
	EXPECT_EQ(std::get<std::string>(object->entries->entry->value->value), "\"Value1\"");

	EXPECT_EQ(object->entries->rhs->entry->key, "\"Key2\"");
	EXPECT_EQ(std::get<bool>(object->entries->rhs->entry->value->value), true);
}