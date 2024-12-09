#pragma once
#include "TexturedModel.h"

class SkyBox : public TexturedModel {
public:
	SkyBox(const std::string& posx, const std::string& negx, const std::string& posy, const std::string& negy, const std::string& posz, const std::string& negz);
};
