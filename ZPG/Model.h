#pragma once
#include <GL/glew.h>

//Include the standard C++ headers  
#include <stdlib.h>
#include <stdio.h>

class Model {
public:
    Model(float* vertices, int size, int count);

    void draw();
private:
    GLuint VAO, VBO;
    int vertexCount;
};