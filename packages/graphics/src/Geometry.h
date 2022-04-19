#pragma once
#include <map>
#include <memory>
#include <vector>
#include <string>

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

	struct Texture {
		std::string mimeType;
		std::shared_ptr<BufferView> image;

		Texture(std::string mimeType, std::shared_ptr<BufferView> image) : mimeType(mimeType), image(image) {}
	};

	struct Material {
		std::shared_ptr<Texture> baseColorTexture{};
	};

	using Attributes = std::map<AttributeType, std::shared_ptr<Accessor>>;
	Attributes attributes;
	std::shared_ptr<Accessor> indices;
	std::shared_ptr<Material> material;

	Geometry(Attributes&& attributes, std::shared_ptr<Accessor> indices, std::shared_ptr<Material> material)
		: attributes(attributes), indices(indices), material(material) {};

	static Geometry CreatePlane(float width, float height);
};