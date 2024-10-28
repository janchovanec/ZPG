#include "ShaderProgram.h"
#include <glm/fwd.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp> // glm::value_ptr
#include "Shader.h"



ShaderProgram::ShaderProgram(const char* vertex_source, const char* fragment_source) {
	fragmentShader = new Shader(GL_FRAGMENT_SHADER, fragment_source);
	vertexShader = new Shader(GL_VERTEX_SHADER, vertex_source);
    program = linkProgram(vertexShader->getShader(), fragmentShader->getShader());

	idViewTransform = glGetUniformLocation(program, "viewMatrix");
	idModelTransform = glGetUniformLocation(program, "modelMatrix");
	idProjectionTransform = glGetUniformLocation(program, "projectionMatrix");
}

ShaderProgram::~ShaderProgram() {
	delete fragmentShader;
	delete vertexShader;
	glDeleteProgram(program);
}

void ShaderProgram::use(glm::mat4 modelMatrix) {
	const auto pers = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.001f, 100000.0f);
	auto mat = modelMatrix;
    glUseProgram(program);
    glUniformMatrix4fv(idProjectionTransform, 1, GL_FALSE, glm::value_ptr(pers));
    glUniformMatrix4fv(idModelTransform, 1, GL_FALSE, glm::value_ptr(mat));
}

void ShaderProgram::updateCamera(const glm::mat4& viewMatrix, const glm::vec3& cameraPos)
{
    glUseProgram(program);
	glUniformMatrix4fv(idViewTransform, 1, GL_FALSE, glm::value_ptr(viewMatrix));
}

GLuint ShaderProgram::linkProgram(GLuint vertexShader, GLuint fragmentShader) {
    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    checkLinkErrors(shaderProgram);
    return shaderProgram;
}

void ShaderProgram::checkLinkErrors(GLuint program) {
    GLint success;
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
        GLchar infoLog[1024];
        glGetProgramInfoLog(program, 1024, NULL, infoLog);
        fprintf(stderr, "Error linking program: %s\n", infoLog);
    }
}