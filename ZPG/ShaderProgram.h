#pragma once
#include <glm/glm.hpp>
#include "Shader.h"
#include "Camera.h"

class ShaderProgram : public ICameraObserver {
public:
	ShaderProgram(const char* vertex_source, const char* fragment_source);
	ShaderProgram() = default;
	~ShaderProgram();
    void use(glm::mat4 modelMatrix);
	void updateCamera(const glm::mat4& viewMatrix, const glm::vec3& cameraPos) override;
private:
	GLuint program;

	GLint idModelTransform;
	GLint idViewTransform;
	GLint idProjectionTransform;

	Shader* vertexShader;

	Shader* fragmentShader;

    GLuint linkProgram(GLuint vertexShader, GLuint fragmentShader);

    void checkLinkErrors(GLuint program);
};