#pragma once

// Dont change numbers, they are hardcoded in shader
enum ELightType {
	DIRECTIONAL = 0,
	POINT = 1,
	SPOT = 2,
	EMPTY = 3,
};

struct Vertex
{
	float Position[3];
	float Normal[3];
	float Texture[2];
	float Tangent[3];
};