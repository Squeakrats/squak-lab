#include "gltf.h"
#include <assert.h>
#include <fstream>

namespace gltf {

void AssertSystemIsLittleEndian() {
	uint32_t i = 0xFFFFFF00;
	assert(*reinterpret_cast<uint8_t*>(&i) == 0x00);
}

json::Object Parse(const std::vector<uint8_t>& buffer) {
	// TODO - Remove assertion of little endian system
	AssertSystemIsLittleEndian();

	assert(buffer[0] == 'g');
	assert(buffer[1] == 'l');
	assert(buffer[2] == 'T');
	assert(buffer[3] == 'F');
	assert(*reinterpret_cast<const uint32_t*>(&buffer[4]) == 2);

	uint32_t chunkLength = *reinterpret_cast<const uint32_t*>(&buffer[12]);
	assert(*reinterpret_cast<const uint32_t*>(&buffer[16]) == gltf::binary::JSON);

	std::string source(buffer.begin() + 20, buffer.begin() + 20 + chunkLength);

	return json::Parse(source);
}

json::Object Load(std::string path) {
	std::ifstream file(path, std::ios::binary);

	// TODO - dont copy memory like this
	std::vector<uint8_t> buffer{};
	while (!file.eof()) { buffer.push_back(file.get()); }

	return Parse(buffer);
}

};
