#pragma once
#include <assert.h>
#include <string>
#include <format>

void Log(std::string&& message);
void Log(const char* message);

#define Assert(condition, message) if (!(condition)) { Log(message); assert(false); std::abort(); }
#define Panic(message) Assert(false, message)