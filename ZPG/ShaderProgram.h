#pragma once
#include <glm/glm.hpp>
#include <vector>
#include "ShaderLoader.h"
#include "IObserver.h"

class Scene;

class ShaderProgram : public IObserver, public ShaderLoader {
public:
	ShaderProgram(const char* vertex_source, const char* fragment_source, Scene* scene, glm::vec3 color = glm::vec3(1.0, 1.0, 1.0));
	ShaderProgram() = default;
	~ShaderProgram();
    void use();
	void updateObserver(ESubjectType type) override;

	void setModelTransform(glm::mat4 modelTransform);
	void setColor(glm::vec3 color);
private:
	GLuint program;
	GLint idLightCount;
	GLint idViewPosition;
	GLint idObjectColor;
	GLint shininess;

	GLint idModelTransform;
	GLint idViewTransform;
	GLint idProjectionTransform;

	Scene* scene;


    GLuint linkProgram(GLuint vertexShader, GLuint fragmentShader);

    void checkLinkErrors(GLuint program);
};