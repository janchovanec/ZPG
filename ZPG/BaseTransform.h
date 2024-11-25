#pragma once
#include <glm/glm.hpp>

class BaseTransform {
public:
	virtual glm::mat4 getTransformMatrix() = 0;
};