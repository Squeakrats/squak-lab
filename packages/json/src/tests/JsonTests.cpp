#include <gtest/gtest.h>
#include  "../json.h"

using namespace json;

TEST(JSON, Parse) {
	Object json = Parse(R"(
		{
			"Key1" : "Value1",
			"Key2" :  true,
			"Key3" : 17,
			"Key4" : { "Key1" : true },
			"Key5" : [1, true, false],
			"Key7" : -19.39
		}
	)");

	EXPECT_EQ(json["Key1"].get<std::string>(), "Value1");
	EXPECT_EQ(json["Key2"].get<bool>(), true);
	EXPECT_EQ(json["Key3"].get<double>(), 17);
	EXPECT_EQ(json["Key4"]["Key1"].get<bool>(), true);
	EXPECT_EQ(json["Key5"][0].get<double>(), 1);
	EXPECT_EQ(json["Key5"][1].get<bool>(), true);
	EXPECT_EQ(json["Key5"][2].get<bool>(), false);
	EXPECT_EQ(json["Key7"].get<double>(), -19.39);
}