#pragma once
#include "Buffer.h"

struct BufferView {
  std::shared_ptr<Buffer> buffer;
  size_t offset;
  size_t length;

  BufferView(std::shared_ptr<Buffer> buffer, size_t offset, size_t length)
    : buffer(buffer)
    , offset(offset)
    , length(length) {}
};