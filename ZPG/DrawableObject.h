#pragma once
#include <memory>
#include <glm/common.hpp>
#include "ShaderProgram.h"
#include "Model.h"
#include "TexturedModel.h"
#include "CompTransform.h"
#include "SkyBox.h"

class DrawableObject {
public:
	DrawableObject(std::shared_ptr<ShaderProgram> shader, Model* model, glm::vec3 color) : shader(shader), model(model), color(color) {
		modelMatrix = CompTransform();
	}
	DrawableObject(std::shared_ptr<ShaderProgram> shader, TexturedModel* model) : shader(shader), model(model), color(color) {
		modelMatrix = CompTransform();
		color = glm::vec3(1.0, 1.0, 1.0);
	}
	DrawableObject(std::shared_ptr<ShaderProgram> shader, Model* model) : shader(shader), model(model) {
		modelMatrix = CompTransform();
		color = glm::vec3(1.0, 1.0, 1.0);
	}

	DrawableObject(Model* model) : model(model) {
		modelMatrix = CompTransform();
		shader = nullptr;
		color = glm::vec3(1.0, 1.0, 1.0);
	}
	DrawableObject() {
		modelMatrix = CompTransform();
		shader = nullptr;
		model = nullptr;
		color = glm::vec3(1.0, 1.0, 1.0);
	}
	void draw();
	void setShader(std::shared_ptr<ShaderProgram> shader);
	CompTransform& getModelMatrix();
private:
	std::shared_ptr<ShaderProgram> shader;
	Model* model;
	CompTransform modelMatrix;
	glm::vec3 color;
};