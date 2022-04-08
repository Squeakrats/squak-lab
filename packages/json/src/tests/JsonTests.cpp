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

	Object json = Create(rawJSON);

	EXPECT_EQ(std::get<std::string>(json.entries.at("Key1")), "Value1");
	EXPECT_EQ(std::get<bool>(json.entries.at("Key2")), true);
}