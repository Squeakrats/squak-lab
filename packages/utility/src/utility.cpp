#include "utility.h"
#include <Windows.h>
#include <fstream>
#include <processthreadsapi.h>

std::unique_ptr<std::ostream> logger;

void InitLogger(std::string&& logFile) {
  logger = std::make_unique<std::ofstream>(
    logFile, std::ofstream::out | std::ofstream::trunc);
}

void Log(std::string&& message) {
  *logger << message << std::endl;
}

void Log(const char* message) {
  *logger << message << std::endl;
}

void spawn(std::string path, std::string arguments) {
  std::string command = path + " " + arguments;

  STARTUPINFO startupInfo{};
  PROCESS_INFORMATION processInfo{};
  Assert(CreateProcess(const_cast<char*>(path.c_str()),
                       const_cast<char*>(command.c_str()),
                       nullptr,
                       nullptr,
                       false,
                       0,
                       nullptr,
                       nullptr,
                       &startupInfo,
                       &processInfo),
         "failed to create process");
}