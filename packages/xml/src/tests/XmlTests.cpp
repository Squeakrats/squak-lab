#include "../xml.h"
#include "regex.h"
#include <gtest/gtest.h>

using namespace xml;

TEST(XML, BasicCommentSucceeds) {
  Document document = Parse(R"(<?xml version="1.0" encoding="UTF-8"?>
<registry>
    <comment>
Copyright 2013-2020 The Khronos Group Inc.
SPDX-License-Identifier: Apache-2.0

This file, gl.xml, is the OpenGL and OpenGL API Registry. The canonical
version of the registry, together with documentation, schema, and Python
generator scripts used to generate C header files for OpenGL and OpenGL ES,
can always be found in the Khronos Registry at
https://github.com/KhronosGroup/OpenGL-Registry
    </comment>
    <!-- SECTION: GL type definitions. -->
</registry>
	)");
}

TEST(XML, InlineTextNode) {
  Document document = Parse(R"(<?xml version="1.0" encoding="UTF-8"?>
    <type>typedef unsigned int <name>GLenum</name>;</type>
	)");

  ASSERT_EQ(document.root->children.size(), 3);
  ASSERT_EQ(
    std::dynamic_pointer_cast<TextNode>(document.root->children[0])->text,
    "typedef unsigned int ");
  ASSERT_EQ(std::dynamic_pointer_cast<TextNode>(
              std::dynamic_pointer_cast<Element>(document.root->children[1])
                ->children[0])
              ->text,
            "GLenum");
  ASSERT_EQ(
    std::dynamic_pointer_cast<TextNode>(document.root->children[2])->text,
    ";");
}

TEST(XML, Attribute) {
  Document document = Parse(R"(<?xml version="1.0" encoding="UTF-8"?>
     <type name="khrplatform"></type>
	)");

  ASSERT_EQ(document.root->attributes["name"], "khrplatform");
}
TEST(XML, Attributes) {
  Document document = Parse(R"(<?xml version="1.0" encoding="UTF-8"?>
     <type a="b" c="d" />
	)");

  ASSERT_EQ(document.root->attributes["a"], "b");
  ASSERT_EQ(document.root->attributes["c"], "d");
}
