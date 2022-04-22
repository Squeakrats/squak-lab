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

    float normalBuffer[] = {
        0.0,  0.0, 1.0,
        0.0,  0.0, 1.0,
        0.0,  0.0, 1.0,
        0.0,  0.0, 1.0,
    };

    uint16_t indexBuffer[] = {
        0, 1, 2,
        0, 2, 3
    };

    std::shared_ptr<BufferView::Buffer> buffer = std::make_shared<BufferView::Buffer>();
    buffer->resize(sizeof(positionBuffer) + + sizeof(normalBuffer) + sizeof(indexBuffer));
    std::memcpy(buffer->data(), positionBuffer, sizeof(positionBuffer));
    std::memcpy(buffer->data() + sizeof(positionBuffer), normalBuffer, sizeof(normalBuffer));
    std::memcpy(buffer->data() + sizeof(positionBuffer) + sizeof(normalBuffer), indexBuffer, sizeof(indexBuffer));

    Attributes attributes = {
        {
            AttributeType::Position,
            std::make_shared<BufferAccessor>(BufferAccessor{
                BufferAccessor::Type::Vector3,
                BufferAccessor::ComponentType::Float,
                4,
                std::make_shared<BufferView>(BufferView{ buffer, 0, sizeof(positionBuffer) })
            })
        },
        {
            AttributeType::Normal,
            std::make_shared<BufferAccessor>(BufferAccessor{
                BufferAccessor::Type::Vector3,
                BufferAccessor::ComponentType::Float,
                4,
                std::make_shared<BufferView>(BufferView{ buffer, sizeof(positionBuffer), sizeof(normalBuffer) })
            })
        }
    };

    std::shared_ptr<BufferAccessor> indices = std::make_shared<BufferAccessor>(BufferAccessor{
        BufferAccessor::Type::Scalar,
        BufferAccessor::ComponentType::UnsignedShort,
        4,
        std::make_shared<BufferView>(BufferView{ buffer, sizeof(positionBuffer) + sizeof(normalBuffer), sizeof(indexBuffer) })
    });

    return Geometry(std::move(attributes), indices, nullptr);
}