#include "FirstPersonCamera.h"


FirstPersonCamera::FirstPersonCamera(Model* Object, glm::vec3 worldUp, float height, float width) {
	//init camera pos
	this->cameraPos = glm::vec3(0, 0, 15.f); //default eye is 0, 0, 10

	this->projectionMatrix = glm::perspective(
		glm::radians(60.f), //FOV
		height / width, //aspect ratio
		0.1f, //ZNear > 0
		100.f
	);

	this->FogMax = 90;
	this->FogMin = 0.01f;

	this->worldUp = worldUp;
}

void FirstPersonCamera::processEvents(glm::vec3 modelPos, glm::vec3 objDir) {
	this->updateCamera(modelPos, objDir);
}

void FirstPersonCamera::updateCamera(glm::vec3 modelPos, glm::vec3 objDir) {
	this->cameraPos[0] = modelPos.x;
	this->cameraPos[1] = modelPos.y;
	this->cameraPos[2] = modelPos.z;

	this->viewMatrix = glm::lookAt(this->cameraPos, { modelPos.x + objDir.x*2,  modelPos.y + objDir.y * 2, modelPos.z + objDir.z * 2 }, this->worldUp);
	//updates to be at model 
	
	
}


