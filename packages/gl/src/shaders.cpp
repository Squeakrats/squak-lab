#include "shaders.h"

namespace gl::shaders {
    
const char* const vertex = R"ESC(#version 300 es
precision highp float;
layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTextureCoordinate;

uniform mat4 uPerspective;
uniform mat4 uView;
uniform mat4 uModel;

out vec2 vTextureCoordinate;

void main() {
    vTextureCoordinate = aTextureCoordinate;
    gl_Position = uPerspective * uView * uModel * vec4(aPosition, 1.0);
}
)ESC";

const char* const fragment = R"ESC(#version 300 es
precision highp float;

in vec2 vTextureCoordinate;

uniform sampler2D uTexture;

out vec4 position;

void main() {
    position = vec4(texture2D(uTexture, vTextureCoordinate).xyz, 1.0);
}
)ESC";

};