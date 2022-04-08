#pragma once
#include <string>

#include "parser.h"

namespace json {

class JSON {
public:
	ast::Object object;

	static JSON Create(std::string source);
};


};