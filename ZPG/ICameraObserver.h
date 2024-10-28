#pragma once
#include <glm/fwd.hpp>

class ICameraObserver
{
public:
	virtual ~ICameraObserver() = default;
	virtual void updateCamera(const glm::mat4& viewMatrix, const glm::vec3& cameraPos) = 0;
};