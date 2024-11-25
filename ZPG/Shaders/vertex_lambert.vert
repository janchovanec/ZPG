#version 330 core

layout(location = 0) in vec3 aPos;       // Position attribute
layout(location = 1) in vec3 aNormal;    // Normal attribute

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

out vec3 FragPos;
out vec3 Normal;

void main()
{
    FragPos = vec3(modelMatrix * vec4(aPos, 1.0));
    Normal = mat3(transpose(inverse(modelMatrix))) * aNormal; // Transform normal to world space

    gl_Position = projectionMatrix * viewMatrix * vec4(FragPos, 1.0);
}
