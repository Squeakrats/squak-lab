#pragma once
#include <squak/core/Buffer.h>

namespace compression {

size_t inflate(Buffer& output, Buffer& input);

};
