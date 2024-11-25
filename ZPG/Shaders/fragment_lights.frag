#version 330 core
#define MAX_LIGHTS 32

struct Light {
    int type; // 0 = directional, 1 = point, 2 = spot, 3 = empty
    vec3 position;
    vec3 color;
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float constant;
    float linear;
    float quadratic;
    float cutOff;
};

uniform int numLights;                 // Number of active lights
uniform Light lights[MAX_LIGHTS];      // Array of light sources (max = MAX_LIGHTS)
uniform Light flashlight;              // Flashlight

uniform vec3 viewPos;                  // Camera/view position
uniform vec3 objectColor;              // Color of the object

uniform float shininess;               // Global shininess coefficient for specular (global = 32.0)

in vec3 FragPos;    // Position of the fragment in world space
in vec3 Normal;     // Normal of the fragment in world space

out vec4 FragColor; // Final color output of the fragment

vec3 directionalLight(int i) {
    // ambient
    vec3 ambient = lights[i].ambient * objectColor;

    // diffuse
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(-lights[i].direction);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lights[i].diffuse * objectColor;

    // Specular
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
    vec3 specular = spec * lights[i].specular * objectColor;

    return (ambient + diffuse + specular);
}

vec3 pointLight(int i) {
    // Ambient
    vec3 ambient = lights[i].ambient * objectColor;

    // Diffuse
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lights[i].position - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lights[i].diffuse * objectColor;

    // Specular
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
    vec3 specular = spec * lights[i].specular * objectColor;

    // Attenuation
    float distance = length(lights[i].position - FragPos);
    float attenuation = 1.0 / (1.0 + lights[i].constant + lights[i].quadratic * pow(distance, 2));


    return (ambient + diffuse + specular) * attenuation;
}

vec3 spotLight(int i) {
    vec3 lightDir = normalize(lights[i].position - FragPos);
    float theta = dot(lightDir, normalize(-lights[i].direction));

    if (theta > lights[i].cutOff) {
        // Ambient
        vec3 ambient = lights[i].ambient;

        // Diffuse
        vec3 norm = normalize(Normal);
        float diff = max(dot(norm, lightDir), 0.0);
        vec3 diffuse = diff * lights[i].diffuse * objectColor;

        // Specular
        vec3 viewDir = normalize(viewPos - FragPos);
        vec3 reflectDir = reflect(-lightDir, norm);
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
        vec3 specular = spec * lights[i].specular * objectColor;

        // Attenuation
        float distance = length(lights[i].position - FragPos);
        float attenuation = 1.0 / (lights[i].constant + lights[i].linear * distance + lights[i].quadratic * pow(distance, 2));

        return (ambient + diffuse + specular) * attenuation;

    }

    return vec3(lights[i].ambient * objectColor);
}

vec3 flashlightUpdate() {
    vec3 lightDir = normalize(flashlight.position - FragPos);
    float theta = dot(lightDir, normalize(-flashlight.direction));

    if (theta > flashlight.cutOff) {
        // Ambient
        vec3 ambient = flashlight.ambient;

        // Diffuse
        vec3 norm = normalize(Normal);
        float diff = max(dot(norm, lightDir), 0.0);
        vec3 diffuse = diff * flashlight.diffuse * objectColor;

        // Specular
        vec3 viewDir = normalize(viewPos - FragPos);
        vec3 reflectDir = reflect(-lightDir, norm);
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
        vec3 specular = spec * flashlight.specular * objectColor;

        // Attenuation
        float distance = length(flashlight.position - FragPos);
        float attenuation = 1.0 / (flashlight.constant + flashlight.linear * distance + flashlight.quadratic * pow(distance, 2));

        return (ambient + diffuse + specular) * objectColor * attenuation;

    }

    return vec3(flashlight.ambient * objectColor);
}

void main() {
    vec3 accumulatedLight = vec3(0.0);
    if (numLights > 0) {
        for (int i = 0; i < numLights; i++) {
            switch (lights[i].type) {
                case 0:
                    accumulatedLight += directionalLight(i);
                    break;
                case 1:
                    accumulatedLight += pointLight(i);
                    break;
                case 2:
                    accumulatedLight += spotLight(i);
                    break;
                case 3:
                    break;
            }
        }
    }

    accumulatedLight += flashlightUpdate();

    FragColor = vec4(accumulatedLight, 1.0);
}
