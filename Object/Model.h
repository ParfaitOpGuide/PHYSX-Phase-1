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

	float lifetime;
	float currTime = 0;

	float damping = 0.9f;
	float Mass;
	P6::MyVector Velocity, Acceleration, velStart, velEnd;

	P6::MyVector accumulatedForce = P6::MyVector(0, 0, 0);
	bool isDestroyed;

	float thetaX, thetaY, thetaZ;
	//model data required for drawing, same for all models so its inherited
	GLuint texture;
	std::vector<float> fullVertexData;
	GLuint VAO, VBO;

	glm::vec3 color;
	std::string name;

	glm::vec3 objDir;
	float pitch, yaw;
public:



	Model(GLFWwindow* window, glm::vec3 vecPos, GLuint shaderProg, std::string objStr, std::string texStr, std::string normStr, float scale, glm::vec3 color);
	Model(GLFWwindow* window, glm::vec3 vecPos, GLuint shaderProg, std::string objStr, std::string texStr, float scale, glm::vec3 color);

	Model(const Model& copy) :
		x(copy.x), y(copy.y), z(copy.z),
		scale(copy.scale), opacity(copy.opacity),
		lifetime(copy.lifetime), currTime(copy.currTime),
		damping(copy.damping), Mass(copy.Mass),
		Velocity(copy.Velocity), Acceleration(copy.Acceleration), velStart(copy.velStart), velEnd(copy.velEnd),
		accumulatedForce(copy.accumulatedForce), isDestroyed(copy.isDestroyed),
		thetaX(copy.thetaX), thetaY(copy.thetaY), thetaZ(copy.thetaZ),
		texture(copy.texture), fullVertexData(copy.fullVertexData),
		VAO(copy.VAO), VBO(copy.VBO),
		color(copy.color), name(copy.name) {} //sometimes you look at something and regret all of your life decisions that led up to that point

	void explodeAndDie();

	void AddForce(P6::MyVector force);
	void ResetForce();


	void Position(float x, float y, float z);

	void UpdateVelocity(float time);
	void UpdatePosition(float time);
	void Update(float time);

	void Destroy();
	bool IsDestroyed()
	{
		return isDestroyed;
	}

	void processEvents(glm::mat4 identity_matrix4,
		GLuint shaderProg,
		MyCamera* Camera,
		PointLight* Point, DirectionLight* Direction);

	void draw(glm::mat4 identity_matrix4,
		GLuint shaderProg,
		MyCamera* Camera,
		PointLight* Point, DirectionLight* Direction);
};

