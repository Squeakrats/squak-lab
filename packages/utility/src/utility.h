#pragma once
#include <assert.h>
#include <format>
#include <string>

void InitLogger(std::string&& logFile);
void Log(std::string&& message);
void Log(const char* message);

void spawn(std::string path, std::string arguments);

#define Assert(condition, message)                                             \
  if (!(condition)) {                                                          \
    Log(message);                                                              \
    assert(false);                                                             \
    std::abort();                                                              \
  }
#define Panic(message) Assert(false, message)