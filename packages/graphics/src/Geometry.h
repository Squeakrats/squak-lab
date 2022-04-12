#pragma once
#include <map>
#include <memory>
#include <vector>

class Geometry {
public:
	using Buffer = std::vector<uint8_t>;
	
	struct BufferView {
		std::shared_ptr<Buffer> buffer;
		size_t offset;
		size_t length;
	};

	enum class AccessorType {
		Scalar,
		Vector2,
		Vector3
	};

	enum class ComponentType {
		Float,
		UnsignedInt,
		UnsignedShort
	};

	struct Accessor {
		AccessorType type;
		ComponentType componentType;
		size_t count;
		std::shared_ptr<BufferView> view;
	};

	enum class AttributeType {
		Position,
		Normal,
		TextureCoordinate_0
	};

	using Attributes = std::map<AttributeType, std::shared_ptr<Accessor>>;
	Attributes attributes;
	std::shared_ptr<Accessor> indices;

	Geometry(Attributes&& attributes, std::shared_ptr<Accessor> indices) : attributes(attributes), indices(indices) {};

	static Geometry CreatePlane(float width, float height);
};