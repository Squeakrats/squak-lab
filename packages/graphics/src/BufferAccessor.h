#pragma once
#include <BufferView.h>
#include <memory>

struct BufferAccessor {
  enum class Type { Scalar, Vector2, Vector3, Vector4 };

  enum class ComponentType { Float, UnsignedInt, UnsignedShort };

  Type type;
  ComponentType componentType;
  size_t count;
  std::shared_ptr<BufferView> view;

  BufferAccessor(Type type,
                 ComponentType componentType,
                 size_t count,
                 std::shared_ptr<BufferView> view)
    : type(type)
    , componentType(componentType)
    , count(count)
    , view(view) {}
};