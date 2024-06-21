
#include "DirectionLight.h"


	DirectionLight::DirectionLight(glm::vec3 lightPos) {
		this->lightPos = lightPos;
		//light color
		this->lightColor = glm::vec3(1, 1, 1);
		this->ambientStr = 0.1f;
		this->ambientColor = this->lightColor;
		this->strength = 0.6;
	}

