#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GLFW/glfw3.h>
#include <vector>
#include "ISubject.h"


class Camera : public ISubject
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
    float MovementSpeed = 2.5f;
    float MouseSensitivity = 0.01f;
    float Zoom = 45.0f;


    Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = 90.0, float pitch = -90.0);

    Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch);

    glm::mat4 GetViewMatrix() const;

    void ProcessKeyboard(int key, float deltaTime);

    void ProcessMouseMovement(float xoffset, float yoffset);

    void ProcessMouseScroll(float yoffset);

	inline glm::vec3 GetPosition() const { return Position; }

	inline void SetPosition(glm::vec3 position) { Position = position; updateCameraVectors(); }
private:
    // calculates the front vector from the Camera's (updated) Euler Angles
    void updateCameraVectors();

};