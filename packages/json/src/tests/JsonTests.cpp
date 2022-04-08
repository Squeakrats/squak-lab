#include <gtest/gtest.h>
#include  "../json.h"

using namespace json;

TEST(JSON, Parse) {
	std::string rawJSON(R"(
		{
			"Key1" : "Value1",
			"Key2" :  true
		}
	)");

	JSON json = JSON::Create(rawJSON);

	EXPECT_EQ(json.object.entries->entry->key, "\"Key1\"");
	EXPECT_EQ(std::get<std::string>(json.object.entries->entry->value->value), "\"Value1\"");

	EXPECT_EQ(json.object.entries->rhs->entry->key, "\"Key2\"");
	EXPECT_EQ(std::get<bool>(json.object.entries->rhs->entry->value->value), true);
}