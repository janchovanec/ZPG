#pragma once
#include "BaseTransform.h"
#include <GLFW/glfw3.h>
#include <glm/ext/matrix_transform.hpp>

class DCirclingTransform : public BaseTransform {
public:
	DCirclingTransform(float radius, float speed) : radius(radius), speed(speed) {}
	inline glm::mat4 getTransformMatrix() {
		float time = glfwGetTime();
		float x = sin(time * speed) * radius;
		float z = cos(time * speed) * radius;

		// rotation
		glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), time * speed + glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));

		// translation
		glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(x, 0.0f, z));

		return translationMatrix * rotationMatrix;
	}
private:
	float radius;
	float speed;
};