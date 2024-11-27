#pragma once

#include "Model.h"

class TexturedModel : public Model {
public:
	TexturedModel(const float* vertices, int size, int count, const char* texturePath);
	virtual void draw() override;
private:
	void setTextureParams(GLint wrapS, GLint wrapT, GLint minFilter, GLint magFilter);
};