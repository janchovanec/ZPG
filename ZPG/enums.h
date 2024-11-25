#pragma once

enum ESubjectType {
	CAMERA,
	LIGHT,
	FLASHLIGHT
};

// Dont change numbers, they are hardcoded in shader
enum ELightType {
	DIRECTIONAL = 0,
	POINT = 1,
	SPOT = 2,
	EMPTY = 3,
};