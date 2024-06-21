#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "MyCamera.h"
#include "../Object/Model.h"
class FirstPersonCamera :
	public MyCamera
{
public:
	//radius size for camera to rotate around the point of the object
	float radius = 18.0f;
	//Modifier for the camera's theta for persp
	float cam_theta_mod = 0;
public:
	FirstPersonCamera(Model* Object, glm::vec3 worldUp, float height, float width);

	void processEvents(glm::vec3 modelPos, glm::vec3 objDir);

	void updateCamera(glm::vec3 modelPos, glm::vec3 objDir);
};

