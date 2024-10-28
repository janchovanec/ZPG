#include "DrawableObject.h"

void DrawableObject::draw()
{
	shader->use(modelMatrix.setPosition(glm::vec3(0.0f, 0.0f, 0.0f)).setRotation(glm::vec3(0.0f, 0.0f, 0.0f)).setScale(glm::vec3(1.0f, 1.0f, 1.0f)).getTransformMatrix());
	model.draw();
}

void DrawableObject::setShader(ShaderProgram* shader)
{
	this->shader = shader;
}

Transform& DrawableObject::getModelMatrix()
{
	return modelMatrix;
}
