#pragma once

#include "Model.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>

class TexturedModel : public Model {
public:
	TexturedModel(const float* vertices, int size, int count, const char* texturePath);
	TexturedModel(const std::string& modelPath);
	virtual void draw() override;
protected:
	TexturedModel(); // for skybox
	GLuint textureID;
private:
	void setTextureParams(GLint wrapS, GLint wrapT, GLint minFilter, GLint magFilter);
	void loadModel(const std::string& path);
	bool assimpModel;
};