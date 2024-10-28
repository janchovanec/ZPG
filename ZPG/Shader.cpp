#include "Shader.h"

Shader::Shader(GLenum type, const char* source) {
	this->type = type;
	shaderID = compileShader(type, source);
}

Shader::~Shader() {
	glDeleteShader(shaderID);
}

GLuint Shader::compileShader(GLenum type, const char* source) {
	GLuint shader = glCreateShader(type);
	glShaderSource(shader, 1, &source, NULL);
	glCompileShader(shader);
	checkCompileErrors(shader, type);
	return shader;
}

void Shader::checkCompileErrors(GLuint shader, GLenum type) {
	GLint success;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success) {
		GLchar infoLog[1024];
		glGetShaderInfoLog(shader, 1024, NULL, infoLog);
		fprintf(stderr, "Error compiling %s shader: %s\n", (type == GL_VERTEX_SHADER ? "VERTEX" : "FRAGMENT"), infoLog);
	}
}