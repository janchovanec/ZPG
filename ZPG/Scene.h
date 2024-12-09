#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdlib.h>
#include <stdio.h>
#include "ShaderProgram.h"
#include <unordered_map>
#include "DrawableObject.h"
#include <string>
#include <vector>
#include <memory>
#include "Camera.h"
#include "Light.h"
#include "SkyBox.h"

class Scene {
public:
	Scene();
	void render(float deltaTime);

	void addObject(DrawableObject object, std::string name);
	void addDirectionalLight(const glm::vec3& direction, const glm::vec3& ambient, const glm::vec3& diffuse, const glm::vec3& specular);
	void addPointLight(const glm::vec3& position, const glm::vec3& ambient, const glm::vec3& diffuse, const glm::vec3& specular, 
		               float constant, float linear, float quadratic);
	void addSpotLight(const glm::vec3& position, const glm::vec3& direction, const glm::vec3& ambient, const glm::vec3& diffuse, 
		              const glm::vec3& specular, float constant, float linear, float quadratic, float cutOff);
	void addShaderProgram(const char* vertex_shader, const char* fragment_shader, glm::vec3 color = glm::vec3(1.0, 1.0, 1.0));
	void addFlashLight(const glm::vec3& position, const glm::vec3& direction, const glm::vec3& ambient, const glm::vec3& diffuse, 
		               const glm::vec3& specular, float constant, float linear, float quadratic, float cutOff);
	void addSkyBox(const std::string& posx, const std::string& negx, const std::string& posy, const std::string& negy, const std::string& posz, const std::string& negz, std::shared_ptr<ShaderProgram> shader);
	
	inline std::shared_ptr<ShaderProgram> getShader(int id) { return shaders[id]; }
	inline DrawableObject& getObject(const std::string& name) { return objects[name]; }


	void movePosition(int key, float deltaTime);
	void moveCamera(double mouseX, double mouseY);
	inline void setCameraPosition(glm::vec3 position) { camera->SetPosition(position); }

	void updateShaders();
	void setProjectionMatrix(const glm::mat4& projection);

	std::vector<std::shared_ptr<Light>> lights;
	Camera* camera;
	Light* flashlight;
	DrawableObject* skybox;
private:
	std::unordered_map<std::string, DrawableObject> objects;
	std::vector<std::shared_ptr<ShaderProgram>> shaders;
	float lastX = 400, lastY = 300;
	bool firstMouse = true;
};
