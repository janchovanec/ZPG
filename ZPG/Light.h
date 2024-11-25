#pragma once

#include <glm/glm.hpp>
#include "ISubject.h"
#include "enums.h"


class Light : public ISubject {
public:
    Light();

	void initDirectional(const glm::vec3& direction, const glm::vec3& ambient, const glm::vec3& diffuse, const glm::vec3& specular);
	void initPoint(const glm::vec3& position, const glm::vec3& ambient, const glm::vec3& diffuse, const glm::vec3& specular, 
				   float constant, float linear, float quadratic);
	void initSpot(const glm::vec3& position, const glm::vec3& direction, const glm::vec3& ambient, const glm::vec3& diffuse, 
				  const glm::vec3& specular, float constant, float linear, float quadratic, float cutOff);
	void initFlashLight(const glm::vec3& position, const glm::vec3& direction, const glm::vec3& ambient, const glm::vec3& diffuse,
						const glm::vec3& specular, float constant, float linear, float quadratic, float cutOff);


    void setPosition(const glm::vec3& position);
    void setColor(const glm::vec3& color);
	void setType(ELightType type);
	void setDirection(const glm::vec3& direction);
	void setAmbient(const glm::vec3& ambient);
	void setDiffuse(const glm::vec3& diffuse);
	void setSpecular(const glm::vec3& specular);
	void setConstant(float constant);
	void setLinear(float linear);
	void setQuadratic(float quadratic);
	void setCutOff(float cutOff);


	inline const glm::vec3& getPosition() const { return position; }
	inline const glm::vec3& getColor() const { return color; }
	inline ELightType getType() const { return type; }
	inline const glm::vec3& getDirection() const { return direction; }
	inline const glm::vec3& getAmbient() const { return ambient; }
	inline const glm::vec3& getDiffuse() const { return diffuse; }
	inline const glm::vec3& getSpecular() const { return specular; }
	inline float getConstant() const { return constant; }
	inline float getLinear() const { return linear; }
	inline float getQuadratic() const { return quadratic; }
	inline float getCutOff() const { return cutOff; }

private:
	ELightType type;
	glm::vec3 color;
	glm::vec3 position; // point, spot
	glm::vec3 direction; // dir, spot
	glm::vec3 ambient; // dir, point, spot
	glm::vec3 diffuse; // dir, point, spot
	glm::vec3 specular; // dir, point, spot

	float constant; // point, spot
	float linear; // point, spot
	float quadratic; // point, spot

	float cutOff; // spot
};

