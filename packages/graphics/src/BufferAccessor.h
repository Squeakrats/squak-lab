#pragma once
#include <memory>
#include <BufferView.h>

struct BufferAccessor {
	enum class Type {
		Scalar,
		Vector2,
		Vector3
	};

	enum class ComponentType {
		Float,
		UnsignedInt,
		UnsignedShort
	};

	Type type;
	ComponentType componentType;
	size_t count;
	std::shared_ptr<BufferView> view;
};