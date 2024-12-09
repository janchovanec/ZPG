#include "ShaderProgram.h"
#include <glm/fwd.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Scene.h"

ShaderProgram::ShaderProgram(const char* vertex_source, const char* fragment_source, Scene* scene, glm::vec3 color) {
	this->scene = scene;
	program = loadShader(vertex_source, fragment_source);
	glUseProgram(program);

	idViewTransform = glGetUniformLocation(program, "viewMatrix");
	idModelTransform = glGetUniformLocation(program, "modelMatrix");
	idProjectionTransform = glGetUniformLocation(program, "projectionMatrix");
	idViewPosition = glGetUniformLocation(program, "viewPos");
	idLightCount = glGetUniformLocation(program, "numLights");
	idObjectColor = glGetUniformLocation(program, "objectColor");


	shininess = glGetUniformLocation(program, "shininess");
	glUniform1f(shininess, 32.0f);

	glUniform3fv(idViewPosition, 1, glm::value_ptr(scene->camera->GetPosition()));
	glUniform1i(idLightCount, 0);
	glUniform3fv(idObjectColor, 1, glm::value_ptr(color));

	// default projection matrix
    const auto pers = glm::perspective(glm::radians(60.0f), 1200.0f / 800.0f, 0.001f, 100000.0f);
    glUniformMatrix4fv(idProjectionTransform, 1, GL_FALSE, glm::value_ptr(pers));
}

ShaderProgram::~ShaderProgram() {
	glDeleteProgram(program);
}

void ShaderProgram::use() {
    glUseProgram(program);
}

void ShaderProgram::updateObserver()
{
	glUseProgram(program);
	// camera update
	auto viewMatrix = scene->camera->GetViewMatrix();
	glUniformMatrix4fv(idViewTransform, 1, GL_FALSE, glm::value_ptr(viewMatrix));
	// view position update
	glUniform3fv(idViewPosition, 1, glm::value_ptr(scene->camera->GetPosition()));


}

void ShaderProgram::setModelTransform(glm::mat4 modelTransform)
{
	glUseProgram(program);
	glUniformMatrix4fv(idModelTransform, 1, GL_FALSE, glm::value_ptr(modelTransform));
}

void ShaderProgram::setColor(glm::vec3 color)
{
	glUseProgram(program);
	glUniform3fv(idObjectColor, 1, glm::value_ptr(color));
}

void ShaderProgram::setProjectionMatrix(glm::mat4 projection)
{
	glUseProgram(program);
	glUniformMatrix4fv(idProjectionTransform, 1, GL_FALSE, glm::value_ptr(projection));
}

void ShaderProgram::updateLights()
{
	glUseProgram(program);
	for (int i = 0; i < scene->lights.size(); i++) {
		glUniform3fv(glGetUniformLocation(program, ("lights[" + std::to_string(i) + "].position").c_str()), 1, glm::value_ptr(scene->lights[i]->getPosition()));
		glUniform3fv(glGetUniformLocation(program, ("lights[" + std::to_string(i) + "].color").c_str()), 1, glm::value_ptr(scene->lights[i]->getColor()));
		glUniform1i(glGetUniformLocation(program, ("lights[" + std::to_string(i) + "].type").c_str()), scene->lights[i]->getType());
		glUniform3fv(glGetUniformLocation(program, ("lights[" + std::to_string(i) + "].direction").c_str()), 1, glm::value_ptr(scene->lights[i]->getDirection()));
		glUniform3fv(glGetUniformLocation(program, ("lights[" + std::to_string(i) + "].ambient").c_str()), 1, glm::value_ptr(scene->lights[i]->getAmbient()));
		glUniform3fv(glGetUniformLocation(program, ("lights[" + std::to_string(i) + "].diffuse").c_str()), 1, glm::value_ptr(scene->lights[i]->getDiffuse()));
		glUniform3fv(glGetUniformLocation(program, ("lights[" + std::to_string(i) + "].specular").c_str()), 1, glm::value_ptr(scene->lights[i]->getSpecular()));
		glUniform1f(glGetUniformLocation(program, ("lights[" + std::to_string(i) + "].constant").c_str()), scene->lights[i]->getConstant());
		glUniform1f(glGetUniformLocation(program, ("lights[" + std::to_string(i) + "].linear").c_str()), scene->lights[i]->getLinear());
		glUniform1f(glGetUniformLocation(program, ("lights[" + std::to_string(i) + "].quadratic").c_str()), scene->lights[i]->getQuadratic());
		glUniform1f(glGetUniformLocation(program, ("lights[" + std::to_string(i) + "].cutOff").c_str()), scene->lights[i]->getCutOff());
	}
	glUniform1i(idLightCount, scene->lights.size());
}

void ShaderProgram::updateFlashLight() {
	if (scene->flashlight == nullptr) return;

	glUseProgram(program);
	glUniform3fv(glGetUniformLocation(program, "flashlight.position"), 1, glm::value_ptr(scene->flashlight->getPosition()));
	glUniform3fv(glGetUniformLocation(program, "flashlight.color"), 1, glm::value_ptr(scene->flashlight->getColor()));
	glUniform1i(glGetUniformLocation(program, "flashlight.type"), scene->flashlight->getType());
	glUniform3fv(glGetUniformLocation(program, "flashlight.direction"), 1, glm::value_ptr(scene->flashlight->getDirection()));
	glUniform3fv(glGetUniformLocation(program, "flashlight.ambient"), 1, glm::value_ptr(scene->flashlight->getAmbient()));
	glUniform3fv(glGetUniformLocation(program, "flashlight.diffuse"), 1, glm::value_ptr(scene->flashlight->getDiffuse()));
	glUniform3fv(glGetUniformLocation(program, "flashlight.specular"), 1, glm::value_ptr(scene->flashlight->getSpecular()));
	glUniform1f(glGetUniformLocation(program, "flashlight.constant"), scene->flashlight->getConstant());
	glUniform1f(glGetUniformLocation(program, "flashlight.linear"), scene->flashlight->getLinear());
	glUniform1f(glGetUniformLocation(program, "flashlight.quadratic"), scene->flashlight->getQuadratic());
	glUniform1f(glGetUniformLocation(program, "flashlight.cutOff"), scene->flashlight->getCutOff());
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