#pragma once

#include <glm/glm.hpp>

class Light {
public:
	//position of light
	glm::vec3 lightPos;
	//light color
	glm::vec3 lightColor;
	float ambientStr;
	glm::vec3 ambientColor;
	//spec strength and phong
	float specStr;
	float specPhong;
	float strength;


	Light();


};