#include "utility.h"
#include <fstream>

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