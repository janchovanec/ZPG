#pragma once
#include <GL/glew.h>

//Include the standard C++ headers  
#include <stdlib.h>
#include <stdio.h>
#include "ShaderProgram.h"

class Model {
public:
    Model(float* vertices, int size, int count);
	Model() : VAO(0), VBO(0), vertexCount(0) {}

    void draw();
private:
    GLuint VAO, VBO;
    int vertexCount;
};



