#pragma once
#include <vector>
#include <memory>

struct BufferView {
	using Buffer = std::vector<uint8_t>;

	std::shared_ptr<Buffer> buffer;
	size_t offset;
	size_t length;
};