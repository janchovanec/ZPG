#include "Shader.h"


Shader::Shader(const char* vertex_source, const char* fragment_source) {
    GLuint vertexShader = compileShader(GL_VERTEX_SHADER, vertex_source);
    GLuint fragmentShader = compileShader(GL_FRAGMENT_SHADER, fragment_source);
    program = linkProgram(vertexShader, fragmentShader);
}
void Shader::use() {
    glUseProgram(program);
}


GLuint Shader::compileShader(GLenum type, const char* source) {
    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &source, NULL);
    glCompileShader(shader);
    checkCompileErrors(shader, type);
    return shader;
}

GLuint Shader::linkProgram(GLuint vertexShader, GLuint fragmentShader) {
    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    checkLinkErrors(shaderProgram);
    return shaderProgram;
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

void Shader::checkLinkErrors(GLuint program) {
    GLint success;
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
        GLchar infoLog[1024];
        glGetProgramInfoLog(program, 1024, NULL, infoLog);
        fprintf(stderr, "Error linking program: %s\n", infoLog);
    }
}