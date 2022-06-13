#pragma once

class IByteStream {
public:
  virtual ~IByteStream() {}

  virtual size_t GetPosition() = 0;
  virtual void SetPosition(size_t position) = 0;

  virtual char Peek() = 0;
  virtual char Get() = 0;
  virtual void Read(char* destination, size_t length) = 0;
  virtual bool AtEnd() = 0;
};