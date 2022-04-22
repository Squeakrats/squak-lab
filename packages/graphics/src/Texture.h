#pragma once
#include <string>
#include "BufferView.h"

struct Texture {
	std::string mimeType;
	std::shared_ptr<BufferView> image;

	Texture(std::string mimeType, std::shared_ptr<BufferView> image) : mimeType(mimeType), image(image) {}
};