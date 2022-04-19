#pragma once
#include <vector>

namespace png {

struct PNG {
	size_t width;
	size_t height;
	std::vector<uint8_t> data;
};

PNG parse(std::vector<uint8_t>& data);

};