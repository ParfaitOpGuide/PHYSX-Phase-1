#include "ThirdPersonCamera.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
ThirdPersonCamera::ThirdPersonCamera(glm::vec3 worldUp, float height, float width) {
		//init camera pos
		this->cameraPos = glm::vec3(0, 0, 15.f); //default eye is 0, 0, 10

		this->projectionMatrix = glm::perspective(
			glm::radians(60.f), //FOV
			height / width, //aspect ratio
			0.1f, //ZNear > 0
			9000.f
		);

		this->FogMax = 50;
		this->FogMin = 0.01f;

		this->worldUp = worldUp;

		this->cam_theta_mod = 90;
		this->cam_phi_mod = 1.57f;
		this->radius = 2800.0f;
	}

	void ThirdPersonCamera::processEvents(glm::vec3 modelPos, glm::vec3 objDir) {
		this->updateCamera(modelPos, objDir);
	}

	void ThirdPersonCamera::updateCamera(glm::vec3 modelPos, glm::vec3 objDir) {
		this->viewMatrix = glm::lookAt(this->cameraPos, {0,0,0}, -this->worldUp);
		
		this->cameraPos[0] = modelPos.x + sin(this->cam_phi_mod) * cos(this->cam_theta_mod) * radius;
		this->cameraPos[1] = modelPos.y + cos(this->cam_phi_mod) * radius;
		this->cameraPos[2] = modelPos.z + sin(this->cam_phi_mod) * sin(this->cam_theta_mod) * radius;
		
		//this->cameraPos = { 0,14,400 };

	}


