#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include <memory>

#include "BaseTransform.h"

class CompTransform : public BaseTransform {
private:
		std::vector<std::shared_ptr<BaseTransform>> transforms;
public:
	CompTransform() {}
	inline void addTransform(std::shared_ptr<BaseTransform> transform) {
		transforms.push_back(transform);
	}
	inline glm::mat4 getTransformMatrix() override {
		glm::mat4 trans = glm::mat4(1.0f);

		for (auto& transform : transforms) {
			trans *= transform->getTransformMatrix();
		}
		return trans;
	}
};