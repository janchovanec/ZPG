#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "BaseTransform.h"

class PosTransform : public BaseTransform {
protected:
	glm::vec3 position;
public:
	PosTransform(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f)) : position(position) {}
	inline glm::mat4 getTransformMatrix() override {
		glm::mat4 trans = glm::mat4(1.0f);
		trans = glm::translate(trans, position);

		return trans;
	}
};