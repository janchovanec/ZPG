#include "Light.h"

// init everything to 0
Light::Light() : type(ELightType::EMPTY), color(0.0f), position(0.0f), direction(0.0f), ambient(0.0f), diffuse(0.0f), specular(0.0f), constant(0.0f), linear(0.0f), quadratic(0.0f), cutOff(0.0f) {}

void Light::setPosition(const glm::vec3& position) {
    this->position = position;
}

void Light::setColor(const glm::vec3& color) {
    this->color = color;
}

void Light::initDirectional(const glm::vec3& direction, const glm::vec3& ambient, const glm::vec3& diffuse, const glm::vec3& specular) {
	type = ELightType::DIRECTIONAL;
	this->direction = direction;
	this->ambient = ambient;
	this->diffuse = diffuse;
	this->specular = specular;
}

void Light::initPoint(const glm::vec3& position, const glm::vec3& ambient, const glm::vec3& diffuse, const glm::vec3& specular, float constant, float linear, float quadratic) {
	type = ELightType::POINT;
	this->position = position;
	this->ambient = ambient;
	this->diffuse = diffuse;
	this->specular = specular;
	this->constant = constant;
	this->linear = linear;
	this->quadratic = quadratic;
}

void Light::initSpot(const glm::vec3& position, const glm::vec3& direction, const glm::vec3& ambient, const glm::vec3& diffuse, const glm::vec3& specular, float constant, float linear, float quadratic, float cutOff) {
	type = ELightType::SPOT;
	this->position = position;
	this->direction = direction;
	this->ambient = ambient;
	this->diffuse = diffuse;
	this->specular = specular;
	this->constant = constant;
	this->linear = linear;
	this->quadratic = quadratic;
	this->cutOff = cutOff;
}

void Light::initFlashLight(const glm::vec3& position, const glm::vec3& direction, const glm::vec3& ambient, const glm::vec3& diffuse, const glm::vec3& specular, float constant, float linear, float quadratic, float cutOff)
{
	this->initSpot(position, direction, ambient, diffuse, specular, constant, linear, quadratic, cutOff);
}

void Light::setType(ELightType type) {
	this->type = type;
}

void Light::setDirection(const glm::vec3& direction) {
	this->direction = direction;
}

void Light::setAmbient(const glm::vec3& ambient) {
	this->ambient = ambient;
}

void Light::setDiffuse(const glm::vec3& diffuse) {
	this->diffuse = diffuse;
}

void Light::setSpecular(const glm::vec3& specular) {
	this->specular = specular;
}

void Light::setConstant(float constant) {
	this->constant = constant;
}

void Light::setLinear(float linear) {
	this->linear = linear;
}

void Light::setQuadratic(float quadratic) {
	this->quadratic = quadratic;
}

void Light::setCutOff(float cutOff) {
	this->cutOff = cutOff;
}

void Light::updateLight(float deltaTime)
{
	if (movementMethod != nullptr) {
		movementMethod(*this, deltaTime);
	}
}


