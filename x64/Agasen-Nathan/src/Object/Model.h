#pragma once


#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <string>
#include <vector>
#include <chrono>

//GLM HEADERS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "../Light/PointLight.h"
#include "../Light/DirectionLight.h"

#include "../Camera/MyCamera.h"

#include "../tiny_obj_loader.h"
#include "../stb_image.h"

#include "../P6/MyVector.h"

class Model {
public:
	float x,
		y,
		z,
		scale,
		opacity;
	P6::MyVector Velocity, Acceleration, velStart, velEnd;

	float thetaX, thetaY, thetaZ;
	//model data required for drawing, same for all models so its inherited
	GLuint texture;
	std::vector<float> fullVertexData;
	GLuint VAO, VBO;

	bool isDestroyed;

	glm::vec3 color;
	std::string name;

public:
	Model(GLFWwindow* window, glm::vec3 vecPos, GLuint shaderProg, std::string objStr, std::string texStr, std::string normStr, glm::vec3 color);
	Model(GLFWwindow* window, glm::vec3 vecPos, GLuint shaderProg, std::string objStr, std::string texStr, glm::vec3 color); //different constructors based on whether normals are involved or not

	void explodeAndDie();

	virtual void processEvents(glm::mat4 identity_matrix4,
		GLuint shaderProg,
		MyCamera* Camera,
		PointLight* Point, DirectionLight* Direction) = 0;

	virtual void draw(glm::mat4 identity_matrix4,
		GLuint shaderProg,
		MyCamera* Camera, //next is light content
		PointLight* Point, DirectionLight* Direction) = 0;

	void UpdateVelocity(float time);
	void UpdatePosition(float time);
	void Update(float time);

	void Destroy(std::string placement, std::chrono::steady_clock::time_point start_time);
	bool IsDestroyed()
	{
		return isDestroyed;
	}

};

