#pragma once
#include <squak/engine/rpc.h>
#include "utility.h"

RPC RPC::Parse(json::Object&& source) {
  RPC rpc{};
  rpc.id = source["id"].get<std::string>();
  rpc.method = source["method"].get<std::string>();

  if (source.has("arguments")) {
    for (auto& argument : source["arguments"].get<json::Array>()) {
      if (std::holds_alternative<double>(argument.value)) {
        rpc.arguments.push_back(std::get<double>(argument.value));
      } else if (std::holds_alternative<std::string>(argument.value)) {
        rpc.arguments.push_back(std::get<std::string>(argument.value));
      } else if (std::holds_alternative<bool>(argument.value)) {
        rpc.arguments.push_back(std::get<bool>(argument.value));
      } else {
        Panic("Unhandled value");
      }
    }
  }

  return rpc;
}