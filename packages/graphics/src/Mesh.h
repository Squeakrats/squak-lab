#pragma once
#include "BufferAccessor.h"
#include "Material.h"
#include <map>
#include <memory>
#include <string>
#include <vector>

class Mesh {
public:
  struct Geometry {
    std::shared_ptr<BufferAccessor> positions{};
    std::shared_ptr<BufferAccessor> normals{};
    std::shared_ptr<BufferAccessor> textureCoordinatess{};
    std::shared_ptr<BufferAccessor> colors{};
    std::shared_ptr<BufferAccessor> indices{};
    std::shared_ptr<Material> material{};
  };

  std::vector<Geometry> geometries{};

  Mesh() {}
  Mesh(std::vector<Geometry>&& geometries)
    : geometries(geometries) {}

  static Mesh CreatePlane(float width, float height);
};