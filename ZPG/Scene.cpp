#include "Scene.h"
#include <glm/glm.hpp>
#include <math.h>

Scene::Scene() {
	camera = new Camera();
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

void Scene::addShader(ShaderProgram* shader, std::string name) {
	shaders[name] = shader;
	camera->addObserver(shader);
}

void Scene::movePosition(int key, float deltaTime) {
	float speed = 15.5f;
	for (auto& shader : shaders) {
		camera->ProcessKeyboard(key, deltaTime);
	}
}

void Scene::moveCamera(double mouseX, double mouseY) {
	for (auto& shader : shaders) {
		camera->ProcessMouseMovement(mouseX - lastX, mouseY - lastY);
	}
	lastX = mouseX;
	lastY = mouseY;
}