#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <string>
#include <vector>

//GLM HEADERS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "../Light/PointLight.h"
#include "../Light/DirectionLight.h"
#include "Model.h"

#include "../Camera/MyCamera.h"

class ModelExtra : public Model {

private:
	float thetaXLast, thetaYLast, thetaZLast;

public:

	ModelExtra(GLFWwindow* window, glm::vec3 vecPos, GLuint shaderProg, std::string objStr, std::string texStr, float scale, float thetaX, float thetaY, float thetaZ, glm::vec3 color);

	void processEvents(glm::mat4 identity_matrix4,
		GLuint shaderProg,
		MyCamera* Camera,
		PointLight* Point, DirectionLight* Direction);

	void draw(glm::mat4 identity_matrix4,
		GLuint shaderProg,
		MyCamera* Camera,
		PointLight* Point, DirectionLight* Direction);
};