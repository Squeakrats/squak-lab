#include "shaders.h"

namespace gl::shaders {
    
namespace textured {

const char* const vertex = R"ESC(#version 300 es
precision highp float;
in vec3 aPosition;
in vec3 aNormal;
in vec2 aTextureCoordinate;

uniform mat4 uPerspective;
uniform mat4 uView;
uniform mat4 uModel;

out vec3 vPosition;
out vec3 vNormal;
out vec2 vTextureCoordinate;

void main() {
    vec4 position = uView * uModel * vec4(aPosition, 1.0);
    vec4 normal = uModel * vec4(aNormal, 0.0);

    vPosition = position.xyz;
    vNormal = normal.xyz;
    vTextureCoordinate = aTextureCoordinate;
    gl_Position = uPerspective * position;
}
)ESC";

const char* const fragment = R"ESC(#version 300 es
precision highp float;

in vec3 vPosition;
in vec3 vNormal;
in vec2 vTextureCoordinate;

uniform sampler2D uTexture;

layout(location = 0) out vec4 oColor;
layout(location = 1) out vec3 oPosition;
layout(location = 2) out vec3 oNormal;

void main() {
    oColor = vec4(texture(uTexture, vTextureCoordinate.xy).xyz, 1.0);
    oPosition = vPosition;
    oNormal = vNormal;
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

uniform vec3 uLightPosition;

uniform sampler2D uDiffuseTexture;
uniform sampler2D uPositionTexture;
uniform sampler2D uNormalTexture;

out vec4 oColor;

void main() {
    vec3 diffuse = texture(uDiffuseTexture, vTextureCoordinate).xyz;
    vec3 position = texture(uPositionTexture, vTextureCoordinate).xyz;
    vec3 normal = texture(uNormalTexture, vTextureCoordinate).xyz;

    vec3 toLight = normalize(uLightPosition - position);
    float s = max(0.0, dot(toLight, normal));

    oColor = vec4(diffuse * s, 1.0);
}
)ESC";

};

};