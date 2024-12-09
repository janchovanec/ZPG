#include "Scene.h"
#include <glm/glm.hpp>
#include <math.h>

Scene::Scene() {
	camera = new Camera();
	flashlight = nullptr;
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
}

void Scene::render(float deltaTime) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	for (auto& obj : objects) {
		obj.second.draw();
	}

	if (flashlight != nullptr) {
		flashlight->setPosition(camera->GetPosition());
		flashlight->setDirection(camera->GetFront());
		flashlight->updateLight(0);
	}

	for (auto& light : lights) {
		light->updateLight(deltaTime);
	}
	
	for (auto& shader : shaders) {
		shader->updateLights();
		shader->updateFlashLight();
	}

	if (skybox != nullptr) {
		skybox->draw();
	}

}

void Scene::addObject(DrawableObject object, std::string name) {
	objects[name] = object;
}

void Scene::addDirectionalLight(const glm::vec3& direction, const glm::vec3& ambient, const glm::vec3& diffuse, const glm::vec3& specular) {
	std::shared_ptr<Light> light = std::make_shared<Light>();
	light->initDirectional(direction, ambient, diffuse, specular);
	lights.push_back(std::move(light));
}

void Scene::addPointLight(const glm::vec3& position, const glm::vec3& ambient, const glm::vec3& diffuse, const glm::vec3& specular, float constant, float linear, float quadratic) {
	std::shared_ptr<Light> light = std::make_shared<Light>();
	light->initPoint(position, ambient, diffuse, specular, constant, linear, quadratic);
	lights.push_back(std::move(light));
}

void Scene::addSpotLight(const glm::vec3& position, const glm::vec3& direction, const glm::vec3& ambient, const glm::vec3& diffuse, const glm::vec3& specular, float constant, float linear, float quadratic, float cutOff) {
	std::shared_ptr<Light> light = std::make_shared<Light>();
	light->initSpot(position, direction, ambient, diffuse, specular, constant, linear, quadratic, cutOff);
	lights.push_back(std::move(light));
}

void Scene::addFlashLight(const glm::vec3& position, const glm::vec3& direction, const glm::vec3& ambient, const glm::vec3& diffuse, const glm::vec3& specular, float constant, float linear, float quadratic, float cutOff)
{
	this->flashlight = new Light();
	this->flashlight->initSpot(position, direction, ambient, diffuse, specular, constant, linear, quadratic, cutOff);
}

void Scene::addSkyBox(const std::string& posx, const std::string& negx, const std::string& posy, const std::string& negy, const std::string& posz, const std::string& negz, std::shared_ptr<ShaderProgram> shader)
{
	skybox = new DrawableObject(shader, new SkyBox(posx, negx, posy, negy, posz, negz));
}

void Scene::addShaderProgram(const char* vertex_shader, const char* fragment_shader, glm::vec3 color) {
	shaders.push_back(std::make_shared<ShaderProgram>(vertex_shader, fragment_shader, this, color));
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
		shader->updateObserver();
	}
}

void Scene::setProjectionMatrix(const glm::mat4& projection)
{
	for (auto& shader : shaders) {
		shader->setProjectionMatrix(projection);
	}
}
