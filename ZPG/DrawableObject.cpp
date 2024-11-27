#include "DrawableObject.h"

void DrawableObject::draw()
{
	shader->setColor(color);
	shader->setModelTransform(modelMatrix.getTransformMatrix());
	shader->use();
	model->draw();
}

void DrawableObject::setShader(std::shared_ptr<ShaderProgram> shader)
{
	this->shader = shader;
}

CompTransform& DrawableObject::getModelMatrix()
{
	return this->modelMatrix;
}
