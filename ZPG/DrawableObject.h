#pragma once
#include "ShaderProgram.h"
#include "Model.h"
#include "Transform.h"
#include <glm/common.hpp>

class DrawableObject {
public:
	DrawableObject(ShaderProgram* shader, Model model) : shader(shader), model(model) {
		modelMatrix = Transform();
	}
	DrawableObject(Model model) : model(model) {
		modelMatrix = Transform();
		shader = nullptr;
	}
	DrawableObject() {
		modelMatrix = Transform();
		shader = nullptr;
		model = Model();
	}
	void draw();
	void setShader(ShaderProgram* shader);
	Transform& getModelMatrix();
private:
	ShaderProgram* shader;
	Model model;
	Transform modelMatrix;
};