#pragma once
#include <glm/fwd.hpp>

class IObserver
{
public:
	virtual ~IObserver() = default;

	// ESubjectType needed to determine what was changed (light, camera, etc.)
	virtual void updateObserver() = 0;
};