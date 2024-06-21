#include "ModelExtra.h"

	ModelExtra::ModelExtra(GLFWwindow* window, glm::vec3 vecPos, GLuint shaderProg, std::string objStr, std::string texStr, float scale, float thetaX, float thetaY, float thetaZ, glm::vec3 color)
		: Model(window, vecPos, shaderProg, objStr, texStr, color) {
		this->scale = scale;
		this->thetaX = thetaX;
		this->thetaY = thetaY;
		this->thetaZ = thetaZ;

		this->thetaXLast = this->thetaX;
		this->thetaYLast = this->thetaY;
		this->thetaZLast = this->thetaZ;
	}

	void ModelExtra::processEvents(glm::mat4 identity_matrix4,
		GLuint shaderProg,
		MyCamera* Camera,
		PointLight* Point, DirectionLight* Direction) {

		this->draw(identity_matrix4,
			shaderProg,
			Camera,
			Point, Direction);

	}


	void ModelExtra::draw(glm::mat4 identity_matrix4,
		GLuint shaderProg,
		MyCamera* Camera,
		PointLight* Point, DirectionLight* Direction) {
		glUseProgram(shaderProg);
		//combines all rotations before transforming to rotate around a point
		glm::mat4 transformation_matrix = glm::rotate(identity_matrix4, glm::radians(this->thetaX), glm::normalize(glm::vec3(1, 0, 0)))
			* glm::rotate(identity_matrix4, glm::radians(this->thetaY), glm::normalize(glm::vec3(0, 1, 0)))
		* glm::rotate(identity_matrix4, glm::radians(this->thetaZ), glm::normalize(glm::vec3(0, 0, 1)));

		transformation_matrix = glm::translate(transformation_matrix, glm::vec3(this->x, this->y, this->z));

		//scale
		transformation_matrix = glm::scale(transformation_matrix, glm::vec3(this->scale, this->scale, this->scale));

		unsigned int projectionLoc = glGetUniformLocation(shaderProg, "projection");
		glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(Camera->projectionMatrix));

		//grab shader transform variable
		unsigned int transformLoc = glGetUniformLocation(shaderProg, "transform");
		//assign to matrix
		glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transformation_matrix));

		unsigned int viewLoc = glGetUniformLocation(shaderProg, "view");
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(Camera->viewMatrix));

	

		glBindVertexArray(this->VAO);

		glActiveTexture(GL_TEXTURE0);
		//get location of tex 1 in the frag shader
		GLuint tex1Address = glGetUniformLocation(shaderProg, "tex1");
		//tell openGL to use it
		glBindTexture(GL_TEXTURE_2D, this->texture);
		//use the texture at 1
		glUniform1i(tex1Address, 0);

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


		//5 because number of verteces
		glDrawArrays(GL_TRIANGLES, 0, this->fullVertexData.size() / 5);
		//std::cout << Point->lightPos.x << " " << Point->lightPos.y << " " << Point->lightPos.z << std::endl;
	}
