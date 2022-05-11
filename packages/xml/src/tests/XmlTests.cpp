#include <gtest/gtest.h>
#include  "../xml.h"

using namespace xml;

TEST(XML, Parse) {
  XMLElement document = Parse(R"(
		<?xml version="1.0" encoding="UTF-8"?>
	)");

}
