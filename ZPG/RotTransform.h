#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "BaseTransform.h"

class RotTransform : public BaseTransform {
protected:
	glm::vec3 rotation;
public:
	RotTransform(glm::vec3 rotation = glm::vec3(0.0f, 0.0f, 0.0f)) : rotation(rotation) {}
	inline glm::mat4 getTransformMatrix() override {
		glm::mat4 trans = glm::mat4(1.0f);

		trans = glm::rotate(trans, rotation.x, glm::vec3(1, 0, 0));
		trans = glm::rotate(trans, rotation.y, glm::vec3(0, 1, 0));
		trans = glm::rotate(trans, rotation.z, glm::vec3(0, 0, 1));

		return trans;
	}
};