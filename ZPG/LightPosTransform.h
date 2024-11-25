#pragma once

#include <glm/glm.hpp>
#include <memory>
#include "Light.h"
#include "PosTransform.h"

// Light movement follow
class LightPosTransform : public BaseTransform {
public:
    LightPosTransform(std::shared_ptr<Light> light, glm::vec3 offset)
        : light(light), offset(offset) {}

    glm::mat4 getTransformMatrix() override {
        glm::vec3 position = light->getPosition() + offset;
        return glm::translate(glm::mat4(1.0f), position);
    }

private:
    std::shared_ptr<Light> light;
    glm::vec3 offset;
};