#include "utility.h"
#include <algorithm>
#include <array>
#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <vector>
#ifdef EMSCRIPTEN
#include <emscripten.h>
EM_JS(char*, readFileSync, (const char* path), {
  var contents = fs.readFileSync(UTF8ToString(path)).toString();
  var length = lengthBytesUTF8(contents);

  var buffer = _malloc(length);
  stringToUTF8(contents, buffer, length);

  return buffer;
});

EM_JS(void, writeFileSync, (const char* path, const char* contents), {
  fs.writeFileSync(UTF8ToString(path), UTF8ToString(contents));
});

#endif

std::string ReadFile(std::string path) {
#ifdef EMSCRIPTEN
  char* buffer = readFileSync(path.c_str());
  std::string result(buffer);
  delete buffer;

  return result;
#else
  std::ifstream file(path);
  Assert(file.is_open(), "invalid file");

  std::stringstream stream{};
  stream << file.rdbuf();

  return stream.str();
#endif
}

void EmitFile(std::string path, std::string contents) {
#ifdef EMSCRIPTEN
  writeFileSync(path.c_str(), contents.c_str());
#else
  std::ofstream file(path);
  Assert(file.is_open(), "invalid file");

  file << contents;
  file.close();
#endif
}

std::string SCLASS = "SCLASS";
std::string SFUNCTION = "SFUNCTION";

using ParamterInfo = std::pair<std::string, std::string>;
struct MethodInfo {
  std::string result{};
  std::string name{};
  std::vector<ParamterInfo> parameters{};
};

using ClassInfo = std::vector<MethodInfo>;
using ClassTable = std::map<std::string, ClassInfo>;

int main(int argc, char* argv[]) {
  std::string sourcePath = argv[1];
  std::string outPath = argv[2];

  std::string content = ReadFile(sourcePath);

  ClassTable classes{};
  std::string last{};

  const char delimiters[] = " \t\n\r,()";
  char* token = std::strtok(content.data(), delimiters);
  while (token != nullptr) {
    if (std::strcmp(token, SCLASS.data()) == 0) {
      token = std::strtok(nullptr, delimiters);
      token = std::strtok(nullptr, delimiters);

      last = std::string(token);
      classes.insert(std::make_pair(last, ClassInfo{}));
    } else if (std::strcmp(token, SFUNCTION.data()) == 0) {
      MethodInfo info{};
      info.result = std::string(std::strtok(nullptr, delimiters));
      info.name = std::string(std::strtok(nullptr, delimiters));

      token = std::strtok(nullptr, delimiters);
      while (*token != ';') {
        std::string type(token);
        std::string name(std::strtok(nullptr, delimiters));
        token = std::strtok(nullptr, delimiters);

        info.parameters.push_back(std::make_pair(type, name));
      }

      classes[last].push_back(info);
    }

    token = std::strtok(nullptr, delimiters);
  }

  std::stringstream ostream{};

  for (auto& entry : classes) {
    std::string className = entry.first;
    std::string classNameUpper = className;
    std::transform(classNameUpper.begin(),
                   classNameUpper.end(),
                   classNameUpper.begin(),
                   [](int character) { return std::toupper(character); });

    ostream << "#define SCLASS_" << classNameUpper
            << "_GENERATED_HEAD public: \\\n";

    ostream << R"( \
    static RuntimeTypeInfo CreateRuntimeTypeInfo() { \
      RuntimeTypeInfo info = Super::GetRuntimeTypeInfoInstance(); \
)";
    ostream << "      info.id = \"" << className << "\"; \\\n";
    ostream << "      info.create = [](const ActorInitializer& initializer) { "
               "return std::make_shared<"
            << className << ">(initializer); };\\\n";

    for (auto& method : entry.second) {
      ostream << "      info.RegisterMethod(\"" << method.name << "\", &"
              << className << "::" << method.name << "); \\\n";
    }
    ostream << R"(\
      RuntimeTypeInfo::Register(info); \
      return info; \
    } \
      \
    static RuntimeTypeInfo& GetRuntimeTypeInfoInstance() { static RuntimeTypeInfo info = CreateRuntimeTypeInfo(); return info; } \
    virtual RuntimeTypeInfo& GetRuntimeTypeInfo() override { return GetRuntimeTypeInfoInstance(); } \
private: \
 
)";

    ostream << "#define SCLASS_" << classNameUpper << "_GENERATED_BODY \\\n";

    for (auto& method : entry.second) {
      ostream << method.result << " " << className << "::" << method.name
              << "(";
      for (size_t i = 0; i < method.parameters.size(); i++) {
        auto& parameter = method.parameters[i];
        ostream << parameter.first << " " << parameter.second;
        if (i != method.parameters.size() - 1) {
          ostream << ",";
        }
      }

      ostream << ") { this->" << method.name << "_Implementation(";
      for (size_t i = 0; i < method.parameters.size(); i++) {
        auto& parameter = method.parameters[i];
        ostream << parameter.second;
        if (i != method.parameters.size() - 1) {
          ostream << ",";
        }
      }

      ostream << "); } \\\n";
    }
  }

  EmitFile(outPath, ostream.str());

  return 0;
}