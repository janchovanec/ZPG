#pragma once
#include <GL/glew.h>
#include  <GL/GL.h>

//Include the standard C++ headers  
#include <stdlib.h>
#include <stdio.h>

class Shader {
public:
    Shader(const char* vertex_source, const char* fragment_source);
    void use();
private:
	GLuint program;

    GLuint compileShader(GLenum type, const char* source);

    GLuint linkProgram(GLuint vertexShader, GLuint fragmentShader);

    void checkCompileErrors(GLuint shader, GLenum type);

    void checkLinkErrors(GLuint program);
};