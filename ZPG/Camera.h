/*#pragma once
#include <glm/glm.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <GL/glew.h>

class ShaderProgram;
class Camera {
public:
	Camera(ShaderProgram* shader);
	Camera(const Camera&) = delete;
	Camera& operator=(const Camera&) = delete;

	glm::mat4 getViewMatrix();
	glm::mat4 getProjectionMatrix();

	void toFront(float speed, float deltaTime);
	void toBack(float speed, float deltaTime);
	void toLeft(float speed, float deltaTime);
	void toRight(float speed, float deltaTime);
	void processMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true);

	glm::vec3 eye; // Position
	glm::vec3 target; // Front
	glm::vec3 up; // Up
	glm::vec3 right; // Right
private:
	ShaderProgram* shader;
	void updateCameraVectors();

	const float MouseSensitivity;
	float Yaw;
	float Pitch;
};*/

#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GLFW/glfw3.h>
#include <vector>

#include "ICameraObserver.h"


// Default camera values
const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 2.5f;
const float SENSITIVITY = 0.1f;
const float ZOOM = 45.0f;


// An abstract camera class that processes input and calculates the corresponding Euler Angles, Vectors and Matrices for use in OpenGL
class Camera
{
public:
    // camera Attributes
    glm::vec3 Position;
    glm::vec3 Front;
    glm::vec3 Up;
    glm::vec3 Right;
    glm::vec3 WorldUp;
    // euler Angles
    float Yaw;
    float Pitch;
    // camera options
    float MovementSpeed;
    float MouseSensitivity;
    float Zoom;

    // constructor with vectors
    Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH);
    // constructor with scalar values
    Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch);

    // returns the view matrix calculated using Euler Angles and the LookAt Matrix
    glm::mat4 GetViewMatrix();

    // processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
    void ProcessKeyboard(int key, float deltaTime);

    // processes input received from a mouse input system. Expects the offset value in both the x and y direction.
    void ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true);

    // processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
    void ProcessMouseScroll(float yoffset);

	inline void addObserver(ICameraObserver* observer) { observers.push_back(observer); }
	inline void removeObserver(ICameraObserver* observer) { observers.erase(std::remove(observers.begin(), observers.end(), observer), observers.end()); }

private:
    // calculates the front vector from the Camera's (updated) Euler Angles
    void updateCameraVectors();

	std::vector<ICameraObserver*> observers;

	void notifyObservers();
};
#endif CAMERA_H