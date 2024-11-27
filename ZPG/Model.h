#pragma once
#include <GL/glew.h>

//Include the standard C++ headers  
#include <stdlib.h>
#include <stdio.h>
#include "ShaderProgram.h"

class Model {
public:
    Model(const float* vertices, int size, int count);
	Model() : VAO(0), VBO(0), vertexCount(0) {}

    virtual void draw();
protected:
    GLuint VAO, VBO;
    int vertexCount;
};
