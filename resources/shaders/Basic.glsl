#type vertex
#version 330 core

layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 color;
layout (location = 2) in vec2 textureCoords;

out vec3 fragmentColor;
out vec2 fragmentTextureCoords;

uniform float offset;

void main() {
    gl_Position = vec4(pos.x + 10, pos.y + 10, pos.z, 1.0f);
    fragmentColor = color;
    fragmentTextureCoords = textureCoords;
}

#type fragment
#version 330 core

in vec3 fragmentColor;
in vec2 fragmentTextureCoords;

out vec4 fragColor;

uniform sampler2D resTexture;
uniform vec3 col;

void main() {
    // fragColor = texture(resTexture, fragmentTextureCoords);
    fragColor = vec4(col, 1.0f);
}
