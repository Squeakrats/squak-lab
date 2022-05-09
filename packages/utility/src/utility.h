#pragma once
#include <assert.h>
#include <format>
#include <string>

void Log(std::string&& message);
void Log(const char* message);

#define Assert(condition, message)                                             \
  if (!(condition)) {                                                          \
    Log(message);                                                              \
    assert(false);                                                             \
    std::abort();                                                              \
  }
#define Panic(message) Assert(false, message)