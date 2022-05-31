#include "utility.h"
#include "xml.h"
#include <fstream>
#include <iostream>
#include <set>
#include <sstream>
#include <vector>

struct Command {
  std::string ret{};
  std::string name{};
  std::vector<std::string> params{};
};

struct Registry {
  std::vector<std::string> types{};
  std::vector<std::pair<std::string, std::string>> enums{};
  std::vector<Command> commands{};
};

std::string SanitizeText(std::string& text) {
  std::stringstream out;

  for (size_t i = 0; i < text.size(); i++) {
    if (text[i] != '&') {
      out << text[i];
    } else if (i + 2 < text.size() && text[i + 1] == 'g' &&
               text[i + 2] == 't') {
      out << '>';
      i += 3;
    } else if (i + 2 < text.size() && text[i + 1] == 'l' &&
               text[i + 2] == 't') {
      out << '<';
      i += 3;
    } else {
      Panic("Unhandled special character");
    }
  }

  return out.str();
}

std::string format(std::string source, std::vector<std::string> replacements) {
  std::stringstream formatted{};
  size_t size = 0;
  size_t replace = 0;

  for (size_t i = 0; i < source.size() - 1; i++) {
    if (source[i] == '{' && source[i + 1] == '}') {
      formatted << source.substr(size, i - size);
      formatted << replacements[replace++];
      i = size = i + 2;
    }
  }

  formatted << source.substr(size, source.size() - size);

  return formatted.str();
}

std::string Stringify(xml::Element& type) {
  std::string result{};
  for (auto& child : type.children) {
    switch (child->type) {
      case xml::NodeType::Text:
        result += SanitizeText(static_pointer_cast<xml::TextNode>(child)->text);
        break;
      case xml::NodeType::Element:
        result += Stringify(*static_pointer_cast<xml::Element>(child));
        break;
      default:
        Panic("Unhandled node type");
    }
  }

  return result;
}

std::vector<std::string> ParseTypes(xml::Element& types) {
  std::vector<std::string> result{};
  for (auto& child : types.children) {
    if (child->type == xml::NodeType::Element) {
      result.push_back(Stringify(*static_pointer_cast<xml::Element>(child)));
    }
  }

  return result;
}

std::vector<std::pair<std::string, std::string>> ParseEnums(
  xml::Element& enums) {
  std::vector<std::pair<std::string, std::string>> result{};
  for (auto& child : enums.children) {
    if (child->type == xml::NodeType::Element) {
      std::shared_ptr<xml::Element> enumElement =
        static_pointer_cast<xml::Element>(child);
      if (enumElement->tag == "enum") {
        result.push_back(std::make_pair(enumElement->attributes.at("name"),
                                        enumElement->attributes.at("value")));
      }
    }
  }

  return result;
}

size_t FindCommandName(std::string& proto) {
  for (size_t i = proto.size(); i-- > 0;) {
    char c = proto[i];
    if (c == ' ' || c == '*') {
      return i + 1;
    }
  }

  Panic("unhandled proto");
}

Command ParseCommand(xml::Element& command) {
  Command result{};
  for (auto& child : command.children) {
    if (child->type != xml::NodeType::Element) {
      continue;
    }

    std::shared_ptr<xml::Element> element =
      static_pointer_cast<xml::Element>(child);

    if (element->tag == "proto") {
      std::string proto = Stringify(*element);
      size_t split = FindCommandName(proto);

      result.ret = proto.substr(0, split);
      result.name = proto.substr(split);
    } else if (element->tag == "param") {
      result.params.push_back(Stringify(*element));
    }
  }

  return result;
}

std::vector<Command> ParseCommands(xml::Element& commands) {
  std::vector<Command> result{};
  for (auto& child : commands.children) {
    if (child->type != xml::NodeType::Element) {
      continue;
    }

    std::shared_ptr<xml::Element> command =
      static_pointer_cast<xml::Element>(child);

    if (command->tag == "command") {
      result.push_back(ParseCommand(*command));
    }
  }

  return result;
}

int main(int argc, char* argv[]) {
  if (argc != 3) {
    std::abort();
  }

  std::string outPath = argv[1];
  std::string xmlPath = argv[2];

  std::ifstream file(xmlPath);
  std::stringstream content{};
  content << file.rdbuf();
  xml::Document document = xml::Parse(content.str());

  Registry registry{};
  for (auto& child : document.root->children) {
    if (child->type != xml::NodeType::Element) {
      continue;
    }

    std::shared_ptr<xml::Element> element =
      static_pointer_cast<xml::Element>(child);

    if (element->tag == "types") {
      auto types = ParseTypes(*element);
      registry.types.insert(registry.types.end(), types.begin(), types.end());
    } else if (element->tag == "enums") {
      auto enums = ParseEnums(*element);
      registry.enums.insert(registry.enums.end(), enums.begin(), enums.end());
    } else if (element->tag == "commands") {
      auto commands = ParseCommands(*element);
      registry.commands.insert(
        registry.commands.end(), commands.begin(), commands.end());
    }
  }

  std::ofstream header(outPath);

  header << "#pragma once" << std::endl;
  header << R"ESC(#ifndef GLR_IMPLEMENTATION
#define GLR_API extern
#else
#define GLR_API
#endif
)ESC";

  for (auto& type : registry.types) {
    header << type << std::endl;
  }

  // only write out the first occurence
  std::set<std::string> enumsWritten{};
  for (auto& enumeration : registry.enums) {
    if (enumsWritten.find(enumeration.first) == enumsWritten.end()) {
      header << "#define " << enumeration.first << " " << enumeration.second
             << std::endl;

      enumsWritten.insert(enumeration.first);
    }
  }

  for (auto& command : registry.commands) {
    header << "GLR_API " << command.ret << " (*" << command.name << ")(";
    for (size_t i = 0; i < command.params.size(); i++) {
      header << command.params[i];
      if (i != command.params.size() - 1) {
        header << ", ";
      }
    }

    header << ");" << std::endl;
  }

  std::stringstream loaders{};
  for (auto& command : registry.commands) {
    loaders << command.name << " = (decltype(" << command.name
            << "))FindProcAddress(\"" << command.name << "\");" << std::endl;
  }

  header << format(R"ESC(#ifdef GLR_IMPLEMENTATION
#include <windows.h>

HMODULE glModule{ };

FARPROC FindProcAddress(const char* name) {
  if (glModule == nullptr) {
    glModule = GetModuleHandle("opengl32.dll");
  }

  FARPROC address = GetProcAddress(glModule, name);
  if (address != nullptr) {
    return address;
  }

  return (FARPROC)wglGetProcAddress(name);
}

void glrInit() {
{}
}

#endif
)ESC",
                   { loaders.str() });

  return 0;
}