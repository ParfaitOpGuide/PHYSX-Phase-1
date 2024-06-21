#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "MyCamera.h"
#include "../Object/Model.h"
class OrthoCamera :
	public MyCamera
{
public:
	glm::vec3 vecOffset;
	
	OrthoCamera(glm::vec3 worldUp);

	void processEvents(glm::vec3 modelPos, glm::vec3 objDir);
	void updateCamera(glm::vec3 modelPos, glm::vec3 objDir);
};

