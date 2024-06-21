
#include "OrthoCamera.h"

	OrthoCamera::OrthoCamera(glm::vec3 worldUp) {
		//init camera pos
		this->cameraPos = glm::vec3(0, 0, 5);

		this->projectionMatrix = glm::ortho(
			-700.f, //Left
			700.f,//Right
			700.f,//Bot
			-700.f,//Top
			-700.f,//Near
			700.f //Far
		);

		this->FogMax = 500;
		this->FogMin = 0.01f;

		this->worldUp = worldUp;
		this->viewMatrix = glm::lookAt(this->cameraPos, {0,0,0}, this->worldUp);
		//for offseting the camera while being up to date with model pos
		this->vecOffset = { 0,0,0 };
	}

	void OrthoCamera::processEvents(glm::vec3 modelPos, glm::vec3 objDir) {
		this->updateCamera(modelPos, objDir);
	}

	void OrthoCamera::updateCamera(glm::vec3 modelPos, glm::vec3 objDir) {
		this->cameraPos[0] = modelPos.x + vecOffset.x;
		this->cameraPos[1] = modelPos.y + 1.f + vecOffset.y;
		this->cameraPos[2] = modelPos.z - 0.01f + vecOffset.z;
		this->viewMatrix = glm::lookAt(this->cameraPos, { modelPos.x + vecOffset.x, modelPos.y + vecOffset.y,  modelPos.z + vecOffset.z }, this->worldUp);
		//updates to be above model, moved around by le offsete

	}


