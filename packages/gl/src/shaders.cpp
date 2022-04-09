#include "shaders.h"

namespace gl::shaders {
    
const char* const vertex = R"ESC(
#version 330 core
in vec2 aPosition;

void main() {
    gl_Position = vec4(aPosition, 0.0, 1.0);
}
)ESC";

const char* const fragment = R"ESC(
#version 330 core

void main() {
    gl_FragColor = vec4(1.0, 0.0, 1.0, 1.0);
}
)ESC";

};