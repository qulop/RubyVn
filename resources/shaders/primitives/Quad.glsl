#type vertex
#version 430 core


layout (location = 0) in vec3 a_pos;
layout (location = 0) in vec3 a_color;

out vec3 vertexColor;


void main() {
    vertexColor = a_color;

    gl_Position = vec4(a_pos, 1.0f);
}
// --------------------------------

#type fragment
#version 430 core

in vec3 vertexColor;

out vec4 fragmentColor;



void main() {
    fragmentColor = vec4(vertexColor, 1.0f);
}
