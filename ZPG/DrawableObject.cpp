#include "DrawableObject.h"

void DrawableObject::draw()
{
	shader->use(modelMatrix.getTransformMatrix());
	model.draw();
}

void DrawableObject::setShader(ShaderProgram* shader)
{
	this->shader = shader;
}

Transform& DrawableObject::getModelMatrix()
{
	return this->modelMatrix;
}
