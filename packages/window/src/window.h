#pragma once
#include <memory>
#include <string>

namespace window {

struct IWindow {
  virtual ~IWindow(){};

  virtual void Poll() = 0;
  virtual bool ShouldClose() = 0;
  virtual bool GetKey(uint8_t key) = 0;
};

std::shared_ptr<IWindow> Create(std::string name,
                                uint32_t width,
                                uint32_t height);

};
