#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "RotTransform.h"

class DRotTransform : public RotTransform {
private:
	glm::vec3 rotationSpeed;
public:
	DRotTransform(glm::vec3 rotationSpeed = glm::vec3(0.0f, 0.0f, 0.0f)) : rotationSpeed(rotationSpeed) {}
	inline glm::mat4 getTransformMatrix() override {
		rotation += rotationSpeed;

		// Clamp rotation angles to 0-2PI
		rotation.x = fmod(rotation.x, 2 * 3.14159265359);
		rotation.y = fmod(rotation.y, 2 * 3.14159265359);
		rotation.z = fmod(rotation.z, 2 * 3.14159265359);

		return RotTransform::getTransformMatrix();
	}
};