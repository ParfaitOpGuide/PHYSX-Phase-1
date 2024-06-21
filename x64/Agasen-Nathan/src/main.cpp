#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <string>
#include <chrono>
#include <conio.h>
//GLM HEADERS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


#include "Light/Light.h"
#include "Light/PointLight.h"
#include "Light/DirectionLight.h"

#include "Object/ModelExtra.h"
#include "Object/Model3D.h"
#include "Object/Model.h"


#include "Camera/MyCamera.h"
#include "Camera/OrthoCamera.h"
#include "Camera/FirstPersonCamera.h"
#include "Camera/ThirdPersonCamera.h"

#include "ShaderClasses/Shader.h"
#include "P6/MyVector.h"
#include "P6/PhysicsWorld.h"
#include "P6/RenderParticle.h"


#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

using namespace std::chrono_literals;
constexpr std::chrono::nanoseconds timestep(16ms);



//window size`
float width = 600.0f, height = 600.0f;


int camType = 0;
//create cams
//ThirdPersonCamera* ThirdPerson;
//FirstPersonCamera* FirstPerson;
OrthoCamera* Orthographic;
//camera vector to store both cameras
std::vector<MyCamera*> vecCameras;

//lights
PointLight* Point = new PointLight(glm::vec3(-4, 5, 0));
DirectionLight* Direction = new DirectionLight(glm::vec3(0, 20, 0));



//uo direction, usually just 1 in y
glm::vec3 worldUp = glm::vec3(0, 1.0f, 0);


//for mouse movement
bool mouse_start = true;
bool mouse_pause = false;
double mouse_x_last;
double mouse_sens = 0.01;

//for model drawing
std::vector<Model*> vecModels;

Model3D* p1;
Model3D* p2;
Model3D* p3;
Model3D* p4;

//bool to close window with a button because im tired of having to press window when my mouse gets locked
bool close = false;

//light state switch
int lightState = 0;

//speed multiplier for movement
float fSpeed = 2.f;
float fOffset = 0.f;

void Key_Callback(GLFWwindow* window, //pointer to window
	int key, //keycode of press
	int scancode, //physical position of the press
	int action,//either press/release
	int mods) //modifier keys
{
	if (action == GLFW_REPEAT || action == GLFW_PRESS) {
		switch (key) {
		case GLFW_KEY_ESCAPE:
			close = true;
			break;
		}
	}
}

glm::vec3 getAccel(float accel, float pitch, float yaw) {
	return { accel * (sin(pitch) * cos(yaw)), accel * (sin(pitch) * sin(yaw)), accel * cos(pitch) };
}

glm::vec3 getVel(float vel, float pitch, float yaw) {
	return { vel * (sin(pitch) * cos(yaw)), vel * (sin(pitch) * sin(yaw)), vel * cos(pitch) };
}

int main(void)
{
	GLFWwindow* window;


	/* Initialize the library */
	if (!glfwInit())
		return -1;

	float width = 600.0f, height = 600.0f;

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(width, height, "PC01 Agasen Nathaniel Ryan B.", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}



	//IDENTITY MATRIXES
	glm::mat3 identity_matrix3 = glm::mat3(1.0f);
	glm::mat4 identity_matrix4 = glm::mat4(1.0f);

	//create shader
	Shader* ShaderSkybox = new Shader(window, "Shaders/skyblock.vert", "Shaders/skyblock.frag");
	Shader* Shader1 = new Shader(window, "Shaders/sampleNew.vert", "Shaders/sampleNew.frag");
	Shader* ShaderNormals = new Shader(window, "Shaders/normals.vert", "Shaders/normals.frag");

	std::vector<Shader*> vecShaders;
	vecShaders.push_back(ShaderNormals);
	vecShaders.push_back(Shader1);
	//create objects and pass the path to the things needed for it specifically, obj first then tex, then normal if needed

	//model set, -8 is screen edge
	p1 = new Model3D(window, { 340, 340, 201 }, Shader1->shaderProg, "3D/sphere.obj", "3D/sphere.jpg", 20.f, { 1.f,0.f,0.f });
	p2 = new Model3D(window, { -340, 340, 173 }, Shader1->shaderProg, "3D/sphere.obj", "3D/sphere.jpg", 20.f, { 0.f,1.f,0.f });
	p3 = new Model3D(window, { -340, -340, -300 }, Shader1->shaderProg, "3D/sphere.obj", "3D/sphere.jpg", 20.f, { 0.f,0.f,1.f });
	p4 = new Model3D(window, { 340, -340, -150 }, Shader1->shaderProg, "3D/sphere.obj", "3D/sphere.jpg", 20.f, { 1.f,1.f,0.f });


	//ThirdPerson = new ThirdPersonCamera(MainModel, worldUp, height, width);
	//FirstPerson = new FirstPersonCamera(MainModel, worldUp, height, width);
	Orthographic = new OrthoCamera(worldUp);

	//Load shader file into a strin steam
	std::fstream vertSrc("Shaders/sampleNew.vert");
	std::stringstream vertBuff;
	std::fstream skyboxVertSrc("Shaders/skyblock.vert");
	std::stringstream skyboxVertBuff;

	vertBuff << vertSrc.rdbuf();
	skyboxVertBuff << skyboxVertSrc.rdbuf();

	std::string vertS = vertBuff.str();
	const char* v = vertS.c_str();
	std::string skyboxVertS = skyboxVertBuff.str();
	const char* sky_v = skyboxVertS.c_str();

	//same but for frag
	std::fstream fragSrc("Shaders/sampleNew.frag");
	std::stringstream fragBuff;
	std::fstream skyboxFragSrc("Shaders/skyblock.frag");
	std::stringstream skyboxFragBuff;

	fragBuff << fragSrc.rdbuf();
	skyboxFragBuff << skyboxFragSrc.rdbuf();

	std::string fragS = fragBuff.str();
	const char* f = fragS.c_str();
	std::string skyboxFragS = skyboxFragBuff.str();
	const char* sky_f = skyboxFragS.c_str();



	/* Make the window's context current */
	glfwMakeContextCurrent(window);
	gladLoadGL();

	glViewport(0, 0, width, height);


	//set the callback function to the window
	glfwSetKeyCallback(window, Key_Callback);


	//blend yo mama
	glEnable(GL_BLEND);

	glBlendFunc(GL_SRC_ALPHA, //source factor
		GL_ONE_MINUS_SRC_ALPHA);

	//camera stuff gets pushed into the vector
	vecCameras.push_back(Orthographic);

	//vecCameras.push_back(ThirdPerson);
	//vecCameras.push_back(FirstPerson);


	//init obj
	vecModels.push_back(p1);

	P6::PhysicsWorld pWorld = P6::PhysicsWorld();

	/* set copypaste just to see
	p1 = new Model3D(window, { 340, 340, 201 }, Shader1->shaderProg, "3D/sphere.obj", "3D/sphere.jpg", 20.f, { 1.f,0.f,0.f });
	p2 = new Model3D(window, { -340, 340, 173 }, Shader1->shaderProg, "3D/sphere.obj", "3D/sphere.jpg", 20.f, { 0.f,1.f,0.f });
	p3 = new Model3D(window, { -340, -340, -300 }, Shader1->shaderProg, "3D/sphere.obj", "3D/sphere.jpg", 20.f, { 0.f,0.f,1.f });
	p4 = new Model3D(window, { 340, -340, -150 }, Shader1->shaderProg, "3D/sphere.obj", "3D/sphere.jpg", 20.f, { 1.f,1.f,0.f });
	*/

	float yaw = atan2(p1->y, p1->x);
	float pitch = atan2(sqrt(p1->y * p1->y + p1->x * p1->x), p1->z) + 3.14;
	p1->Acceleration = P6::MyVector(getAccel(14.5, pitch, yaw));
	p1->Velocity = P6::MyVector(getVel(80, pitch, yaw));
	p1->name = "Red";
	p1->velStart = p1->Velocity;
	pWorld.AddParticle(p1);

	yaw = atan2(p2->y, p2->x);
	pitch = atan2(sqrt(p2->y * p2->y + p2->x * p2->x), p2->z) + 3.14;
	p2->Acceleration = P6::MyVector(getAccel(8, pitch, yaw));
	p2->Velocity = P6::MyVector(getVel(90, pitch, yaw));
	p2->name = "Green";
	p2->velStart = p2->Velocity;
	pWorld.AddParticle(p2);

	yaw = atan2(p3->y, p3->x);
	pitch = atan2(sqrt(p3->y * p3->y + p3->x * p3->x), p3->z) + 3.14;
	p3->Acceleration = P6::MyVector(getAccel(1, pitch, yaw));
	p3->Velocity = P6::MyVector(getVel(130, pitch, yaw));
	p3->name = "Blue";
	p3->velStart = p3->Velocity;
	pWorld.AddParticle(p3);

	yaw = atan2(p4->y, p4->x);
	pitch = atan2(sqrt(p4->y * p4->y + p4->x * p4->x), p4->z) + 3.14;
	p4->Acceleration = P6::MyVector(getAccel(3, pitch, yaw));
	p4->Velocity = P6::MyVector(getVel(110, pitch, yaw));
	p4->name = "Yellow";
	p4->velStart = p4->Velocity;
	pWorld.AddParticle(p4);


	//clock and var
	using clock = std::chrono::high_resolution_clock;
	auto curr_time = clock::now();
	auto prev_time = curr_time;
	std::chrono::nanoseconds curr_ns(0);
	pWorld.start_time = clock::now();

	bool printFlag = false;
	bool startFlag = false;
	bool endFlag = false; //flags that check that the ball has entered the screen, then hit the ground

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window) && !close)
	{
		//get current time
		curr_time = clock::now();
		//duration between this and last
		auto dur = std::chrono::duration_cast<std::chrono::nanoseconds> (curr_time - prev_time);
		//prev to current for next iteration
		prev_time = curr_time;

		curr_ns += dur;

		if (curr_ns >= timestep && !endFlag) {
			if (p1->y >= -340 && !startFlag) {
				startFlag = true;
			}


			//nano to millie
			auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(curr_ns);
			//reset
			curr_ns -= curr_ns;

			pWorld.Update((float)ms.count() / 1000);

			if (p1->y <= -340 && startFlag) {
				auto end_time = clock::now();

				if (!printFlag) {

					printFlag = true;
				}

				p1->y = -340;
				p1->Acceleration = P6::MyVector(0, 0, 0);
				p1->Velocity = P6::MyVector(0, 0, 0);

			}
		}
		//end clock

		/* Render here */
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//processes events depending on camtype
	//	vecCameras[camType]->processEvents({ -2 * vecModels[0]->x,  -2 * vecModels[0]->y, -2*vecModels[0]->z }, MainModel->objDir);


		glDepthMask(GL_TRUE);
		glDepthFunc(GL_LESS);

		int shaderCount;
		//pass all necessary drawing stuff to the objects draw, call all objects draw in vector
		for (auto i : pWorld.ParticleDraw) {


			//std::cout << shaderCount << std::endl;
			i->processEvents(identity_matrix4,
				ShaderNormals->shaderProg,
				vecCameras[camType],
				Point, Direction);
		}

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}

	for (int i = 0; i < vecModels.size(); i++) {
		vecModels[i]->explodeAndDie(); //deletes all vaos since we cant do that from main anymore
	}


	glfwTerminate();
	std::cout << "\nPress any key to end the program: ";
	_getch();

	return 0;
}