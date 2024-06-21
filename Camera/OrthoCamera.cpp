
#include "OrthoCamera.h"

OrthoCamera::OrthoCamera(glm::vec3 worldUp) {
	//init camera pos
	this->cameraPos = glm::vec3(0, 0, 5);

	this->projectionMatrix = glm::ortho(
		-800.f, //Left
		800.f,//Right
		800.f,//Bot
		-800.f,//Top
		-8000.f,//Near
		8000.f //Far
	);

	this->FogMax = 500;
	this->FogMin = 0.01f;

	this->worldUp = worldUp;
	this->viewMatrix = glm::lookAt(this->cameraPos, { 0,0,0 }, this->worldUp);
	//for offseting the camera while being up to date with model pos
	this->vecOffset = { 0,0,0 };

	this->cam_theta_mod = 90;
	this->cam_phi_mod = 1.57f;
	this->radius = 2800.0f;
}

void OrthoCamera::processEvents(glm::vec3 modelPos, glm::vec3 objDir) {
	this->updateCamera(modelPos, objDir);
}

void OrthoCamera::updateCamera(glm::vec3 modelPos, glm::vec3 objDir) {
	this->viewMatrix = glm::lookAt(this->cameraPos, { 0,0,0 }, this->worldUp);

	this->cameraPos[0] = modelPos.x + sin(this->cam_phi_mod) * cos(this->cam_theta_mod) * radius;
	this->cameraPos[1] = modelPos.y + cos(this->cam_phi_mod) * radius;
	this->cameraPos[2] = modelPos.z + sin(this->cam_phi_mod) * sin(this->cam_theta_mod) * radius;
	//updates to be above model, moved around by le offsete

}


