#pragma once
#include <glm/fwd.hpp>
#include "enums.h"

class IObserver
{
public:
	virtual ~IObserver() = default;

	// ESubjectType needed to determine what was changed (light, camera, etc.)
	virtual void updateObserver(ESubjectType type) = 0;
};