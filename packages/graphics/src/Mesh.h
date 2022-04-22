#pragma once
#include <map>
#include <memory>
#include <vector>
#include <string>
#include "BufferAccessor.h"
#include "Material.h"

class Mesh {
public:
	enum class AttributeType {
		Position,
		Normal,
		TextureCoordinate_0
	};

	using Geometry = std::map<AttributeType, std::shared_ptr<BufferAccessor>>;

	Geometry geometry;
	std::shared_ptr<BufferAccessor> indices;
	std::shared_ptr<Material> material;

	Mesh(Geometry&& geometry, std::shared_ptr<BufferAccessor> indices, std::shared_ptr<Material> material)
		: geometry(geometry), indices(indices), material(material) {};

	static Mesh CreatePlane(float width, float height);
};