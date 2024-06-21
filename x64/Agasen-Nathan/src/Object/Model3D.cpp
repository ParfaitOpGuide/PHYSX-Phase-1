
#include "Model3D.h"


Model3D::Model3D(GLFWwindow* window, glm::vec3 vecPos, GLuint shaderProg, std::string objStr, std::string texStr, std::string normStr, float scale, glm::vec3 color)
	: Model(window, vecPos, shaderProg, objStr, texStr, normStr, color) {
	this->scale = scale;
	this->thetaX = 0.f;
	this->thetaY = 0.f;
	this->thetaZ = 0.f;
	//pitch is vert yaw is hori
	this->pitch = 0;
	this->yaw = 90;
}

Model3D::Model3D(GLFWwindow* window, glm::vec3 vecPos, GLuint shaderProg, std::string objStr, std::string texStr, float scale, glm::vec3 color)
	: Model(window, vecPos, shaderProg, objStr, texStr, color) {
	this->scale = scale;
	this->thetaX = 0.f;
	this->thetaY = 0.f;
	this->thetaZ = 0.f;

	this->pitch = 0;
	this->yaw = 90;
}

void Model3D::processEvents(glm::mat4 identity_matrix4,
	GLuint shaderProg,
	MyCamera* Camera,
	PointLight* Point, DirectionLight* Direction) {
	this->draw(identity_matrix4,
		shaderProg,
		Camera,
		Point, Direction);
}

void Model3D::draw(glm::mat4 identity_matrix4,
	GLuint shaderProg,
	MyCamera* Camera,
	PointLight* Point, DirectionLight* Direction) {
	glUseProgram(shaderProg);

	//transformation before the yaw rots to rotate the actual thingymabob
	glm::mat4 transformation_matrix = glm::translate(identity_matrix4, glm::vec3(-this->x, -this->y, -this->z));

	transformation_matrix = glm::rotate(transformation_matrix, glm::radians(this->thetaX), glm::normalize(glm::vec3(1, 0, 0)))
		* glm::rotate(transformation_matrix, glm::radians(this->yaw), glm::normalize(glm::vec3(0, 1, 0)))
		* glm::rotate(transformation_matrix, glm::radians(this->thetaZ), glm::normalize(glm::vec3(0, 0, 1)));

	transformation_matrix = glm::translate(transformation_matrix, glm::vec3(this->x, this->y, this->z));

	
	//scale
	transformation_matrix = glm::scale(transformation_matrix, glm::vec3(this->scale, this->scale, this->scale));

	this->objDir = { transformation_matrix[2][0], transformation_matrix[2][1], transformation_matrix[2][2] };
	unsigned int projectionLoc = glGetUniformLocation(shaderProg, "projection");
	glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(Camera->projectionMatrix));

	//grab shader transform variable
	unsigned int transformLoc = glGetUniformLocation(shaderProg, "transform");
	//assign to matrix
	glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transformation_matrix));

	unsigned int viewLoc = glGetUniformLocation(shaderProg, "view");
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(Camera->viewMatrix));
	// offset slightly to be in front of the ship
	Point->updatePos(glm::vec3(transformation_matrix[3][0] += 12*this->objDir.x, transformation_matrix[3][1], transformation_matrix[3][2]));
	
	//std::cout << transformation_matrix[3][0] << std::endl;
	glBindVertexArray(this->VAO);

	glActiveTexture(GL_TEXTURE0);
	//get location of tex 0 in the frag shader
	GLuint tex0Address = glGetUniformLocation(shaderProg, "tex0");
	//tell openGL to use it
	glBindTexture(GL_TEXTURE_2D, this->texture);
	//use the texture at 0fu
	glUniform1i(tex0Address, 0);

	GLuint cameraPosAddress = glGetUniformLocation(shaderProg, "cameraPos");
	glUniform3fv(cameraPosAddress, 1, glm::value_ptr(Camera->cameraPos));
	//point
//light assigning
	GLuint Point_lightAddress = glGetUniformLocation(shaderProg, "Point_lightPos");
	glUniform3fv(Point_lightAddress, 1, glm::value_ptr(Point->lightPos));
	GLuint Point_lightColorAddress = glGetUniformLocation(shaderProg, "Point_lightColor");
	glUniform3fv(Point_lightColorAddress, 1, glm::value_ptr(Point->lightColor));
	//ambient assigning
	GLuint Point_ambientStrAddress = glGetUniformLocation(shaderProg, "Point_ambientStr");
	glUniform1f(Point_ambientStrAddress, Point->ambientStr);
	GLuint Point_ambientColorAddress = glGetUniformLocation(shaderProg, "Point_ambientColor");
	glUniform3fv(Point_ambientColorAddress, 1, glm::value_ptr(Point->ambientColor));
	//specular assigning
	GLuint Point_specStrAddress = glGetUniformLocation(shaderProg, "Point_specStr");
	glUniform1f(Point_specStrAddress, Point->specStr);
	GLuint Point_specPhongAddress = glGetUniformLocation(shaderProg, "Point_specPhong");
	glUniform1f(Point_specPhongAddress, Point->specPhong);
	GLuint Point_strengthAddress = glGetUniformLocation(shaderProg, "Point_strength");
	glUniform1f(Point_strengthAddress, Point->strength);

	//Dir
	//light assigning
	GLuint Direction_lightAddress = glGetUniformLocation(shaderProg, "Dir_lightPos");
	glUniform3fv(Direction_lightAddress, 1, glm::value_ptr(Direction->lightPos));
	GLuint Direction_lightColorAddress = glGetUniformLocation(shaderProg, "Dir_lightColor");
	glUniform3fv(Direction_lightColorAddress, 1, glm::value_ptr(Direction->lightColor));
	//ambient assigning
	GLuint Direction_ambientStrAddress = glGetUniformLocation(shaderProg, "Dir_ambientStr");
	glUniform1f(Direction_ambientStrAddress, Direction->ambientStr);
	GLuint Direction_ambientColorAddress = glGetUniformLocation(shaderProg, "Dir_ambientColor");
	glUniform3fv(Direction_ambientColorAddress, 1, glm::value_ptr(Direction->ambientColor));
	//specular assigning
	GLuint Direction_specStrAddress = glGetUniformLocation(shaderProg, "Dir_specStr");
	glUniform1f(Direction_specStrAddress, Direction->specStr);
	GLuint Direction_specPhongAddress = glGetUniformLocation(shaderProg, "Dir_specPhong");
	glUniform1f(Direction_specPhongAddress, Direction->specPhong);
	GLuint Direction_strengthAddress = glGetUniformLocation(shaderProg, "Dir_strength");
	glUniform1f(Direction_strengthAddress, Direction->strength);

	//pass fog vals relating to camera
	GLuint FogMaxAddress = glGetUniformLocation(shaderProg, "FogMax");
	glUniform1f(FogMaxAddress, Camera->FogMax);
	GLuint FogMinAddress = glGetUniformLocation(shaderProg, "FogMin");
	glUniform1f(FogMinAddress, Camera->FogMin);
	GLuint OpacityAddress = glGetUniformLocation(shaderProg, "opacity");
	glUniform1f(OpacityAddress, this->opacity);

	GLuint unlitColorAddress = glGetUniformLocation(shaderProg, "unlitColor");
	glUniform3fv(unlitColorAddress, 1, glm::value_ptr(this->color));

	//5 because number of verteces
	glDrawArrays(GL_TRIANGLES, 0, this->fullVertexData.size() / 5);
	
}
