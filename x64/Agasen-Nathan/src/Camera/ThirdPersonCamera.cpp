#include "ThirdPersonCamera.h"


ThirdPersonCamera::ThirdPersonCamera(Model* Object, glm::vec3 worldUp, float height, float width) {
		//init camera pos
		this->cameraPos = glm::vec3(0, 0, 15.f); //default eye is 0, 0, 10

		this->projectionMatrix = glm::perspective(
			glm::radians(60.f), //FOV
			height / width, //aspect ratio
			0.1f, //ZNear > 0
			100.f
		);

		this->FogMax = 50;
		this->FogMin = 0.01f;

		this->worldUp = worldUp;
		//Camera View Matrix
		this->viewMatrix = glm::lookAt(this->cameraPos, { -2 * Object->x,  Object->y, -2 * Object->z }, worldUp);
	}

	void ThirdPersonCamera::processEvents(glm::vec3 modelPos, glm::vec3 objDir) {
		this->updateCamera(modelPos, objDir);
	}

	void ThirdPersonCamera::updateCamera(glm::vec3 modelPos, glm::vec3 objDir) {
		this->viewMatrix = glm::lookAt(this->cameraPos, { modelPos.x,  modelPos.y, modelPos.z }, this->worldUp);
	//updates to be behind model a bit
		this->cameraPos[0] = modelPos.x + sin(this->cam_theta_mod) * radius;
		this->cameraPos[1] = modelPos.y + 1.f;
		this->cameraPos[2] = modelPos.z + cos(this->cam_theta_mod) * radius;
	}


