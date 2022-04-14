#include "utility.h"
#include <iostream>

void Log(std::string&& message) {
	std::cout << message << std::endl;
}

void Log(const char* message) {
	std::cout << message << std::endl;
}