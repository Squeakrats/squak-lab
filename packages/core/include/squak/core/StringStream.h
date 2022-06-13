#pragma once
#include "utility.h"
#include <squak/core/IByteStream.h>
#include <string>

class StringStream : public IByteStream {
private:
  std::string buffer;
  size_t position = 0;

public:
  StringStream(std::string&& buffer) : buffer(buffer) {}

  virtual size_t GetPosition() override { return this->position; }
  virtual void SetPosition(size_t position) override {
    Assert(position <= this->buffer.size(), "invalid position");
    this->position = position;
  }

  virtual char Peek() override {
    Assert(this->position < this->buffer.size(), "invalid position");
    return this->buffer[this->position];
  }

  virtual char Get() override {
    Assert(this->position < this->buffer.size(), "invalid position");
    return this->buffer[this->position++];
  }

  virtual void Read(char* destination, size_t length) override {
    Assert(this->position + length - 1 < this->buffer.size(), "invalid position");
    for (size_t i = 0; i < length; i++) {
      destination[i] = this->buffer[position++];
    }
  }

  virtual bool AtEnd() override {
    return this->position == this->buffer.size();
  }
};