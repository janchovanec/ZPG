#pragma once
#include <glm/glm.hpp>
#include <GL/glew.h>

class Shader {
private:
	GLuint shaderID;
	GLenum type;
	GLuint compileShader(GLenum type, const char* source);
	void checkCompileErrors(GLuint shader, GLenum type);
public:
	Shader(GLenum type, const char* source);
	Shader(const Shader&) = delete;
	Shader& operator=(const Shader&) = delete;
	~Shader();

	inline GLuint getShader() { return shaderID; }
	inline GLenum getType() { return type; }

};