#pragma once
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

#include "Light.h"

class PointLight : public Light {
public:


public:
	PointLight(glm::vec3 lightPos);

	void updatePos(glm::vec3 lightPos);

	void setColor(glm::vec3 lightColor);
};