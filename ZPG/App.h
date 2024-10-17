#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdlib.h>
#include <stdio.h>
#include "Shader.h"
#include "Model.h"

class App {
public:
    App();

    ~App();

    void run();

private:
    GLFWwindow* window;
    Shader* shader1;
    Shader* shader2;
    Model* model1;
    Model* model2;

    void initGLFW();

    void initGLEW();

    void createShaders();

    void createModels();

    void render();
};