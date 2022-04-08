#include <gtest/gtest.h>
#include  "../json.h"

using namespace json;

TEST(JSON, Parse) {
	Object json = Create(R"(
		{
			"Key1" : "Value1",
			"Key2" :  true
		}
	)");

	EXPECT_EQ(json["Key1"].AsString(), "Value1");
	EXPECT_EQ(json["Key2"].AsBool(), true);
}