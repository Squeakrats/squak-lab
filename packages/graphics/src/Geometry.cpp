#include "Geometry.h"
#include <iterator>

Geometry Geometry::CreatePlane(float width, float height) {
    float hw = width / 2;
    float hh = height / 2;

    float positionBuffer[] = {
         hw,  hh, 0.0,
        -hw,  hh, 0.0,
        -hw, -hh, 0.0,
         hw, -hh, 0.0
    };

    uint16_t indexBuffer[] = {
        0, 1, 2,
        0, 2, 3
    };

    std::shared_ptr<Buffer> buffer = std::make_shared<Buffer>();
    buffer->resize(sizeof(positionBuffer) + sizeof(indexBuffer));
    std::memcpy(buffer->data(), positionBuffer, sizeof(positionBuffer));
    std::memcpy(buffer->data() + sizeof(positionBuffer), indexBuffer, sizeof(indexBuffer));

    Attributes attributes = {
        {
            AttributeType::Position,
            std::make_shared<Accessor>(Accessor{
                AccessorType::Vector2,
                ComponentType::Float,
                4,
                std::make_shared<BufferView>(BufferView{ buffer, 0, sizeof(positionBuffer) })
            })
        }
    };

    std::shared_ptr<Accessor> indices = std::make_shared<Accessor>(Accessor{
        AccessorType::Scalar,
        ComponentType::UnsignedInt,
        6,
        std::make_shared<BufferView>(BufferView{ buffer, sizeof(positionBuffer), sizeof(indexBuffer) })
    });

    return Geometry(std::move(attributes), indices);
}