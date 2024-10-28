#include "Transform.h"

Transform::Transform()
    : position(glm::vec3(0.0f, 0.0f, 0.0f)), rotation(glm::vec3(0.0f, 0.0f, 0.0f)), scale(glm::vec3(1.0f, 1.0f, 1.0f)) {
}

Transform& Transform::setPosition(const glm::vec3& pos) {
    position = pos;
    return *this;
}

Transform& Transform::setRotation(const glm::vec3& rot) {
    rotation = rot;
    return *this;
}

Transform& Transform::setScale(glm::vec3 scl) {
    scale = scl;
	return *this;
}

const glm::vec3& Transform::getPosition() const { return position; }
const glm::vec3& Transform::getRotation() const { return rotation; }
const glm::vec3& Transform::getScale() const { return scale; }

glm::mat4 Transform::getTransformMatrix() {
    glm::mat4 trans = glm::mat4(1.0f);
    
    trans = glm::translate(trans, position);
    trans = glm::rotate(trans, rotation.x, glm::vec3(1, 0, 0));
    trans = glm::rotate(trans, rotation.y, glm::vec3(0, 1, 0));
    trans = glm::rotate(trans, rotation.z, glm::vec3(0, 0, 1));
    trans = glm::scale(trans, scale);

	// Clamp rotation angles to 0-2PI
	rotation.x = fmod(rotation.x, 2 * 3.14159265359);
	rotation.y = fmod(rotation.y, 2 * 3.14159265359);
	rotation.z = fmod(rotation.z, 2 * 3.14159265359);
    
    return trans;
}
