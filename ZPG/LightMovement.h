#pragma once

#include <glm/glm.hpp>
#include "Light.h"
#include <GLFW/glfw3.h>

namespace LightMovement
{
    void oscillate(Light& light, float deltaTime) {
        glm::vec3 pos = light.getPosition();
        pos.x += sin(glfwGetTime()) * deltaTime; // Oscillate along the x-axis
        light.setPosition(pos);
    }

    void circularMotion(Light& light, float deltaTime) {
        static float angle = 0.0f;
        angle += deltaTime / 10;
        float radius = 0.002f;
		glm::vec3 pos = glm::vec3(light.getPosition().x + cos(angle) * radius, light.getPosition().y, light.getPosition().z + sin(angle) * radius);
        light.setPosition(pos);
    }
}