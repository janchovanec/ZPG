#include "Scene.h"
#include <glm/glm.hpp>
#include <math.h>

Scene::Scene() {
	camera = new Camera();
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
}

void Scene::render() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	for (auto& obj : objects) {
		obj.second.draw();
	}
}

void Scene::addObject(DrawableObject object, std::string name) {
	objects[name] = object;
}

void Scene::addLight(const glm::vec3& position, const glm::vec3& color)
{
	std::shared_ptr<Light> light = std::make_shared<Light>();
	light->initBasic(position, color);
	lights.push_back(std::move(light));
	for (auto& shader : shaders) {
		lights.back()->addObserver(shader);
	}
}

void Scene::addShaderProgram(const char* vertex_shader, const char* fragment_shader, glm::vec3 color) {
	shaders.push_back(std::make_shared<ShaderProgram>(vertex_shader, fragment_shader, this, color));
	for (auto& light : lights) {
		light->addObserver(shaders.back());
	}
	this->camera->addObserver(shaders.back());
}

void Scene::movePosition(int key, float deltaTime) {
	camera->ProcessKeyboard(key, deltaTime);
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

// Force update of shaders
void Scene::updateShaders()
{
	for (auto& shader : shaders) {
		shader->updateObserver(ESubjectType::LIGHT);
		shader->updateObserver(ESubjectType::CAMERA);
	}
}
