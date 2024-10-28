#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdlib.h>
#include <stdio.h>
#include "ShaderProgram.h"
#include <unordered_map>
#include "DrawableObject.h"
#include <string>
#include "Camera.h"

class Scene {
public:
	Scene();
	void render(float deltaTime);

	void addObject(DrawableObject object, std::string name);

	void addShader(ShaderProgram* shader, std::string name);

	inline ShaderProgram* getShader(const std::string& name) { return shaders[name]; }
	inline DrawableObject& getObject(const std::string& name) { return objects[name]; }

	void movePosition(int key, float deltaTime);
	void moveCamera(double mouseX, double mouseY);

private:
	std::unordered_map<std::string, DrawableObject> objects;
	std::unordered_map<std::string, ShaderProgram*> shaders;
	float lastX = 400, lastY = 300;
	Camera* camera;
	bool firstMouse = true;
};
