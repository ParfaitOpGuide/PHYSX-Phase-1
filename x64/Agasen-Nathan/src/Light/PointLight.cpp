#include "PointLight.h"

	PointLight::PointLight(glm::vec3 lightPos) {
		this->lightPos = lightPos;
		//light color
		this->lightColor = glm::vec3(1, 1, 1);
		this->ambientStr = 0.01f;
		this->ambientColor = this->lightColor;
		this->strength = 5000;

	}

	void PointLight::updatePos(glm::vec3 lightPos) {
		this->lightPos = lightPos;

		//std::cout << this->lightPos[0] << " " << this->lightPos[1] << " " << this->lightPos[2] << " ";
	}

	void PointLight::setColor(glm::vec3 lightColor) {
		this->lightColor = lightColor;
		this->ambientColor = lightColor;
	}
