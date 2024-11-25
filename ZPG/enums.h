#pragma once

enum ESubjectType {
	CAMERA,
	LIGHT
};

// Dont change numbers, they are hardcoded in shader
enum ELightType {
	DIRECTIONAL = 0,
	POINT = 1,
	SPOT = 2,
	FLASHLIGHT = 3,
	EMPTY = 4
};