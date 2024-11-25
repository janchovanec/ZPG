#pragma once
#include <memory>
#include "ShaderProgram.h"
#include "Model.h"
#include "CompTransform.h"
#include <glm/common.hpp>

class DrawableObject {
public:
	DrawableObject(std::shared_ptr<ShaderProgram> shader, Model model, glm::vec3 color) : shader(shader), model(model), color(color), rotationSpeed(glm::vec3(0.0, 0.0, 0.0)) {
		modelMatrix = CompTransform();
	}
	DrawableObject(Model model) : model(model), rotationSpeed(glm::vec3(0.0, 0.0, 0.0)) {
		modelMatrix = CompTransform();
		shader = nullptr;
		color = glm::vec3(1.0, 1.0, 1.0);
	}
	DrawableObject() {
		modelMatrix = CompTransform();
		shader = nullptr;
		model = Model();
		color = glm::vec3(1.0, 1.0, 1.0);
		rotationSpeed = glm::vec3(0.0, 0.0, 0.0);
	}
	void draw();
	void setShader(std::shared_ptr<ShaderProgram> shader);
	inline void setRotationSpeed(glm::vec3 rotationSpeed) { this->rotationSpeed = rotationSpeed; }
	CompTransform& getModelMatrix();
private:
	std::shared_ptr<ShaderProgram> shader;
	Model model;
	CompTransform modelMatrix;
	glm::vec3 color;
	glm::vec3 rotationSpeed;
};