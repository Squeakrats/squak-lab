#include "shaders.h"

namespace gl::shaders {
    
const char* const vertex = R"ESC(
#version 330 core
layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec3 aNormal;

uniform mat4 uPerspective;
uniform mat4 uModel;

varying vec3 vNormal;

void main() {
    vNormal = aNormal;
    gl_Position = uPerspective * uModel * vec4(aPosition, 1.0);
}
)ESC";

const char* const fragment = R"ESC(
#version 330 core

varying vec3 vNormal;

void main() {
    float s = max(0, dot(normalize(vNormal), vec3(0.0, 1.0, 0.0)));
    gl_FragColor = vec4(s * vec3(0.3, 0.2, 0.2) + vec3(0.2, 0.4, 0.6), 1.0);
}
)ESC";

};