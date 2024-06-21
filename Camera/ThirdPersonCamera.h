#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "MyCamera.h"
#include "../Object/Model.h"
class ThirdPersonCamera :
	public MyCamera
{
public:
	
public:
	ThirdPersonCamera(glm::vec3 worldUp, float height, float width);

	void processEvents(glm::vec3 modelPos, glm::vec3 objDir);

	void updateCamera(glm::vec3 modelPos, glm::vec3 objDir);
};

