#version 330 core

struct Light {
    vec3 position;
    vec3 color;
};

uniform int numLights;
uniform Light lights[10]; //Max 10 lights

uniform vec3 objectColor;

in vec3 FragPos;
in vec3 Normal;

out vec4 FragColor;

void main()
{
    vec3 norm = normalize(Normal);
    vec3 color = vec3(0.0);

    for (int i = 0; i < numLights; ++i) {
        vec3 lightDir = normalize(lights[i].position - FragPos);
        float diff = max(dot(norm, lightDir), 0.0);
        vec3 diffuse = diff * lights[i].color;

        // Accumulate all lights
        color += diffuse * objectColor;
    }

    FragColor = vec4(color, 1.0);
}
