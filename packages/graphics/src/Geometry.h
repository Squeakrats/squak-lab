#pragma once
#include <map>
#include <memory>
#include <vector>
#include <string>
#include "BufferAccessor.h"

class Geometry {
public:
	enum class AttributeType {
		Position,
		Normal,
		TextureCoordinate_0
	};

	struct Texture {
		std::string mimeType;
		std::shared_ptr<BufferView> image;

		Texture(std::string mimeType, std::shared_ptr<BufferView> image) : mimeType(mimeType), image(image) {}
	};

	struct Material {
		std::shared_ptr<Texture> baseColorTexture{};
	};

	using Attributes = std::map<AttributeType, std::shared_ptr<BufferAccessor>>;
	Attributes attributes;
	std::shared_ptr<BufferAccessor> indices;
	std::shared_ptr<Material> material;

	Geometry(Attributes&& attributes, std::shared_ptr<BufferAccessor> indices, std::shared_ptr<Material> material)
		: attributes(attributes), indices(indices), material(material) {};

	static Geometry CreatePlane(float width, float height);
};