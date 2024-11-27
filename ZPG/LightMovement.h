#pragma once

#include <glm/glm.hpp>
#include "Light.h"
#include <GLFW/glfw3.h>

namespace LightMovement
{
    void oscillate(Light& light, float deltaTime) {
        glm::vec3 pos = light.getPosition();
        pos.x += sin(glfwGetTime()) * deltaTime;
        light.setPosition(pos);
    }

    void circularMotion(Light& light, float deltaTime) {
        static float angle = 0.0f;
        angle += deltaTime / 10.0 * (1 + (rand() % 100) / 100.0f);
        float radius = 0.002f * (1 + (rand() % 100) / 100.0f);
		glm::vec3 pos = glm::vec3(light.getPosition().x + cos(angle) * radius, light.getPosition().y, light.getPosition().z + sin(angle) * radius);
        light.setPosition(pos);
    }
}