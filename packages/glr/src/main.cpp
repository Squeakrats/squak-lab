#include "utility.h"
#include "xml.h"
#include <fstream>
#include <iostream>
#include <sstream>

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

void WriteType(std::stringstream& out, xml::Element& type) {
  for (auto& child : type.children) {
    switch (child->type) {
      case xml::NodeType::Text:
        out << SanitizeText(static_pointer_cast<xml::TextNode>(child)->text);
        break;
      case xml::NodeType::Element:
        WriteType(out, *static_pointer_cast<xml::Element>(child));
        break;
      default:
        Panic("Unhandled node type");
    }
  }
}

void WriteTypes(std::stringstream& out, xml::Element& types) {
  for (auto& child : types.children) {
    if (child->type != xml::NodeType::Element) {
      continue;
    }
    std::shared_ptr<xml::Element> type =
      static_pointer_cast<xml::Element>(child);

    WriteType(out, *type);
    out << std::endl;
  }
}

void WriteEnums(std::stringstream& out, xml::Element& enums) {
  for (auto& child : enums.children) {
    if (child->type != xml::NodeType::Element) {
      continue;
    }

    std::shared_ptr<xml::Element> enumElement =
      static_pointer_cast<xml::Element>(child);

    if (enumElement->tag != "enum") {
      continue;
    }

    out << "#define " << enumElement->attributes.at("name") << " "
        << enumElement->attributes.at("value") << ";\n";
  }
}

void WriteCommand(std::stringstream& out, xml::Element& command) {
  std::vector<std::shared_ptr<xml::Element>> params{};
  for (auto& child : command.children) {
    if (child->type != xml::NodeType::Element) {
      continue;
    }

    std::shared_ptr<xml::Element> element =
      static_pointer_cast<xml::Element>(child);

    if (element->tag == "proto") {
      WriteType(out, *element);
      out << "(";
    } else if (element->tag == "param") {
      params.push_back(element);
    }
  }

  for (size_t i = 0; i < params.size(); i++) {
    WriteType(out, *params[i]);
    if (i != params.size() - 1) {
      out << ", ";
    }
  }

  out << ")" << std::endl;
}

void WriteCommands(std::stringstream& out, xml::Element& commands) {
  for (auto& child : commands.children) {
    if (child->type != xml::NodeType::Element) {
      continue;
    }

    std::shared_ptr<xml::Element> command =
      static_pointer_cast<xml::Element>(child);

    if (command->tag != "command") {
      continue;
    }

    WriteCommand(out, *command);
    out << std::endl;
  }
}

int main(int argc, char* argv[]) {
  std::ifstream file("C:\\Users\\yusef\\dev\\squak-lab\\assets\\gl.xml");
  std::stringstream content{};
  content << file.rdbuf();

  xml::Document document = xml::Parse(content.str());

  std::stringstream out{};
  for (auto& child : document.root->children) {
    if (child->type != xml::NodeType::Element) {
      continue;
    }

    std::shared_ptr<xml::Element> element =
      static_pointer_cast<xml::Element>(child);

    if (element->tag == "types") {
      WriteTypes(out, *element);
    } else if (element->tag == "enums") {
      WriteEnums(out, *element);
    } else if (element->tag == "commands") {
      WriteCommands(out, *element);
    }
  }

  std::string outContent = out.str();

  return 0;
}