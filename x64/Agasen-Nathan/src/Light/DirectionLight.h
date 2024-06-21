#pragma once

#include "Light.h"

class DirectionLight : public Light {

public:
	DirectionLight(glm::vec3 lightPos);

};