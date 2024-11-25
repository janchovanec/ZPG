#include "Camera.h"

Camera::Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch) : Front(glm::vec3(0.0f, 0.0f, -1.0f)), ISubject(ESubjectType::CAMERA)
{
    Position = position;
    WorldUp = up;
    Yaw = yaw;
    Pitch = pitch;
    updateCameraVectors();
}

Camera::Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch) : Front(glm::vec3(0.0f, 0.0f, -1.0f)), ISubject(ESubjectType::CAMERA)
{
    Position = glm::vec3(posX, posY, posZ);
    WorldUp = glm::vec3(upX, upY, upZ);
    Yaw = yaw;
    Pitch = pitch;
    updateCameraVectors();
}

glm::mat4 Camera::GetViewMatrix() const
{
    auto look = glm::lookAt(Position, Position + Front, Up);
    return look;
}

void Camera::ProcessKeyboard(int key, float deltaTime)
{
    float velocity = MovementSpeed * deltaTime;
    if (key == GLFW_KEY_W)
        Position += Front * velocity;
    if (key == GLFW_KEY_S)
        Position -= Front * velocity;
    if (key == GLFW_KEY_A)
        Position -= Right * velocity;
    if (key == GLFW_KEY_D)
        Position += Right * velocity;

	//Position.y = 0.1f;
	notifyObservers();
}


void Camera::ProcessMouseMovement(float xoffset, float yoffset)
{
    xoffset *= MouseSensitivity;
    yoffset *= MouseSensitivity;

    Yaw += xoffset;
    Pitch += yoffset;

    // make sure that when pitch is out of bounds, screen doesn't get flipped
    if (Pitch > 89.0f)
            Pitch = 89.0f;
    if (Pitch < -89.0f)
        Pitch = -89.0f;
    

	// keep yaw between 0 and 360
	if (Yaw > 360.0f)
		Yaw = 0.0f;
	if (Yaw < 0.0f)
		Yaw = 360.0f;

    // update Front, Right and Up Vectors using the updated Euler angles
    updateCameraVectors();
}


void Camera::ProcessMouseScroll(float yoffset)
{
    Zoom -= (float)yoffset;
    if (Zoom < 1.0f)
        Zoom = 1.0f;
    if (Zoom > 45.0f)
        Zoom = 45.0f;
}

void Camera::updateCameraVectors()
{
    // calculate the new Front vector
    glm::vec3 front;
    front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    front.y = sin(glm::radians(Pitch));
    front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    Front = glm::normalize(front);

    // re-calculate the Right and Up vector
    Right = glm::normalize(glm::cross(Front, WorldUp));  // normalize the vectors
    Up = glm::normalize(glm::cross(Right, Front));

	notifyObservers();
}