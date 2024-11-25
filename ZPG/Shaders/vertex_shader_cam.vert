#version 410
layout(location = 0) in vec3 localPosition;
layout(location = 1) in vec3 vn;
uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;
out vec3 color;
void main(void) { gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(localPosition, 1.0); color = vn; }