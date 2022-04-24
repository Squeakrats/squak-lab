#include "shaders.h"

namespace gl::shaders {
    
namespace textured {

const char* const vertex = R"ESC(#version 300 es
precision highp float;
in vec3 aPosition;
in vec2 aTextureCoordinate;

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
    position = vec4(texture(uTexture, vTextureCoordinate.xy).xyz, 1.0);
}
)ESC";

};

namespace solid {

const char* const vertex = R"ESC(#version 300 es
precision highp float;
in vec3 aPosition;

uniform mat4 uPerspective;
uniform mat4 uView;
uniform mat4 uModel;

void main() {
    gl_Position = uPerspective * uView * uModel * vec4(aPosition, 1.0);
}
)ESC";

const char* const fragment = R"ESC(#version 300 es
precision highp float;

uniform vec3 uColor;

out vec4 position;

void main() {
    position = vec4(uColor, 1.0);
}
)ESC";

};

namespace fullscreen {

const char* const vertex = R"ESC(#version 300 es
precision highp float;
in vec3 aPosition;
in vec2 aTextureCoordinate;

out vec2 vTextureCoordinate;

void main() {
    vTextureCoordinate = aTextureCoordinate;
    gl_Position = vec4(aPosition, 1.0);
}
)ESC";

const char* const fragment = R"ESC(#version 300 es
precision highp float;

in vec2 vTextureCoordinate;

uniform sampler2D uTexture;

out vec4 position;

void main() {
    position = vec4(texture(uTexture, vTextureCoordinate).xyz, 1.0);
}
)ESC";

};

};