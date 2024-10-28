#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Transform {
public:
    Transform();

    // Setters
    Transform& setPosition(const glm::vec3& position);
    Transform& setRotation(const glm::vec3& rotation);
    Transform& setScale(glm::vec3 scale);

    // Getters
    const glm::vec3& getPosition() const;
    const glm::vec3& getRotation() const;
    const glm::vec3& getScale() const;

	// Modify the transformation
	inline void move(const glm::vec3& offset) { position += offset; }
	inline void rotate(const glm::vec3& offset) { rotation += offset; }
	inline void scaleBy(const glm::vec3& offset) { scale += offset; }

    // Retrieve the transformation matrix
    glm::mat4 getTransformMatrix();

private:
    glm::vec3 position;
    glm::vec3 rotation;  // Euler angles (in radians)
    glm::vec3 scale;
};
