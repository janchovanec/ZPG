#include "Scene.h"
#include <glm/glm.hpp>
#include <math.h>

Scene::Scene() {
	camera = new Camera();
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
}

void Scene::render(float deltaTime) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	for (auto& obj : objects) {
		obj.second.draw();
	}
}

void Scene::addObject(DrawableObject object, std::string name) {
	objects[name] = object;
}

void Scene::addShader(ShaderProgram* shader) {
	shaders.push_back(shader);
	camera->addObserver(shader);
}

void Scene::movePosition(int key, float deltaTime) {
	for (auto& shader : shaders) {
		camera->ProcessKeyboard(key, deltaTime);
	}
}

void Scene::moveCamera(double mouseX, double mouseY) {
	float xpos = static_cast<float>(mouseX);
	float ypos = static_cast<float>(mouseY);


	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}


	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos;

	lastX = xpos;
	lastY = ypos;

	camera->ProcessMouseMovement(xoffset, yoffset);
}