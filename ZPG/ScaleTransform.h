#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "BaseTransform.h"

class ScaleTransform : public BaseTransform {
private:
	glm::vec3 scale;
public:
	ScaleTransform(glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f)) : scale(scale) {}
	inline glm::mat4 getTransformMatrix() override {
		glm::mat4 trans = glm::mat4(1.0f);
		trans = glm::scale(trans, scale);

		return trans;
	}
};