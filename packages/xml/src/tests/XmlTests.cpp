#include <gtest/gtest.h>
#include  "../xml.h"

using namespace xml;

TEST(XML, Parse) {
  XMLElement document = Parse(R"(
	<?xml version="1.0" encoding="UTF-8"?>
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
</registry>
	)");
}
