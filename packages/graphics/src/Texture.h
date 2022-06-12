#pragma once
#include <squak/core/BufferView.h>
#include <string>

struct Texture {
  std::string mimeType;
  std::shared_ptr<BufferView> image;

  Texture(std::string mimeType, std::shared_ptr<BufferView> image)
    : mimeType(mimeType)
    , image(image) {}
};