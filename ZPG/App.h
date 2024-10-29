#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdlib.h>
#include <stdio.h>
#include "ShaderProgram.h"
#include "Model.h"
#include "Scene.h"

class App {

public:
    App();
	App(const App&) = delete;
	App& operator=(const App&) = delete;

    ~App();

    void run();

	void initCallback();

	void initScene();

    void cursor_pos_callback(GLFWwindow* window, double mouseX, double mouseY);

	void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

	static App& getInstance() {
		if (instance == nullptr) {
			instance = new App();
		}
		return *instance;
	}
private:
    GLFWwindow* window;
	Scene* scene;


    void initGLFW();

    void initGLEW();

	static App* instance;

	float deltaTime = 0.0f;
};
