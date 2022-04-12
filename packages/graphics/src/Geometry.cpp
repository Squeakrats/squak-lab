#include "Geometry.h"

Geometry Geometry::CreatePlane(float width, float height) {
    float hw = width / 2;
    float hh = height / 2;

    std::vector<float> positions = {
         hw,  hh,
        -hw,  hh,
        -hw, -hh,

         hw,  hh,
        -hw, -hh,
         hw, -hh,
    };

    return Geometry(std::move(positions));
}