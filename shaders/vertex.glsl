#version 330 core

layout(location = 0) in vec3 positionAttr;
layout(location = 1) in vec3 colorAttr;

out vec3 color;

uniform mat4 MVP;

void main() {
    color = colorAttr / 255.0f;
    gl_Position = MVP * vec4(positionAttr, 1.0f);
}
