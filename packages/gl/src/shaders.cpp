#include "shaders.h"

namespace gl::shaders {
    
const char* const vertex = R"ESC(#version 300 es
precision highp float;
layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec3 aNormal;

uniform mat4 uPerspective;
uniform mat4 uModel;

out vec3 vNormal;

void main() {
    vNormal = aNormal;
    gl_Position = uPerspective * uModel * vec4(aPosition, 1.0);
}
)ESC";

const char* const fragment = R"ESC(#version 300 es
precision highp float;

in vec3 vNormal;

out vec4 position;

void main() {
    float s = dot(normalize(vNormal), vec3(0.0, 1.0, 0.0));
    position = vec4(s * vec3(0.3, 0.2, 0.2) + vec3(0.2, 0.4, 0.6), 1.0);
}
)ESC";

};