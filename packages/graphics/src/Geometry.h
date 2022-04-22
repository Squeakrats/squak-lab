#pragma once
#include <map>
#include <memory>
#include <vector>
#include <string>
#include "BufferAccessor.h"
#include "Material.h"

class Geometry {
public:
	enum class AttributeType {
		Position,
		Normal,
		TextureCoordinate_0
	};

	using Attributes = std::map<AttributeType, std::shared_ptr<BufferAccessor>>;
	Attributes attributes;
	std::shared_ptr<BufferAccessor> indices;
	std::shared_ptr<Material> material;

	Geometry(Attributes&& attributes, std::shared_ptr<BufferAccessor> indices, std::shared_ptr<Material> material)
		: attributes(attributes), indices(indices), material(material) {};

	static Geometry CreatePlane(float width, float height);
};