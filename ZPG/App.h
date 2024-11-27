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

	void treeScene();

	void ballScene();

	void triangleScene();

    void cursor_pos_callback(GLFWwindow* window, double mouseX, double mouseY);

	void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

	void updateProjectionMatrix(float width, float height);

	static App& getInstance() {
		if (instance == nullptr) {
			instance = new App();
		}
		return *instance;
	}

	static void win_size_callback(GLFWwindow* window, int width, int height);
private:
    GLFWwindow* window;
	Scene* scene;


    void initGLFW();

    void initGLEW();

	static App* instance;

	float deltaTime = 0.0f;
};
