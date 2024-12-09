#version 330 core

struct Light {
    vec3 position;
    vec3 color;
};

uniform int numLights;                 // Number of active lights
uniform Light lights[10];              // Array of light sources (max 10 lights)

uniform vec3 viewPos;                  // Camera/view position
uniform vec3 objectColor;              // Color of the object
uniform float ambientStrength;         // Strength of ambient light
uniform float specularStrength;        // Strength of specular highlight
uniform float shininess;               // Shininess coefficient for specular

in vec3 FragPos;    // Position of the fragment in world space
in vec3 Normal;     // Normal of the fragment in world space

out vec4 FragColor; // Final color output of the fragment

void main()
{
    vec3 ambient = vec3(0.0);
    vec3 diffuse = vec3(0.0);
    vec3 specular = vec3(0.0);

    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);

    for (int i = 0; i < numLights; ++i) {
        // Ambient lighting
        ambient += ambientStrength * lights[i].color;

        // Diffuse lighting
        vec3 lightDir = normalize(lights[i].position - FragPos);
        float diff = max(dot(norm, lightDir), 0.0);
        diffuse += diff * lights[i].color;

        // Specular lighting
        vec3 reflectDir = reflect(-lightDir, norm);
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
        specular += specularStrength * spec * lights[i].color;
    }

    // Combine results
    vec3 result = (ambient + diffuse + specular) * objectColor;
    FragColor = vec4(result, 1.0);
}
