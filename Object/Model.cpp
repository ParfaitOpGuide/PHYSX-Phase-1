
#include "Model.h"


Model::Model(GLFWwindow* window, glm::vec3 vecPos, GLuint shaderProg, std::string objStr, std::string texStr, std::string normStr, float scale, glm::vec3 color) {
	this->scale = scale;
	this->thetaX = 0.f;
	this->thetaY = 0.f;
	this->thetaZ = 0.f;
	//pitch is vert yaw is hori
	this->pitch = 0;
	this->yaw = 90;

	bool isDestroyed = false;
	this->color = color;

	this->x = vecPos[0];
	this->y = vecPos[1];
	this->z = vecPos[2];

	this->opacity = 1.0f;

	//override regular model class to add normals

	//convert the strings to const char for stbi
	const char* texChar = texStr.c_str();
	const char* normChar = normStr.c_str();

	//TEXTURING
	int img_width, img_height, colorChannels;

	stbi_set_flip_vertically_on_load(true);
	unsigned char* tex_bytes = stbi_load(texChar, &img_width, &img_height, &colorChannels, 0);


	//generate a new reference
	glGenTextures(1, &this->texture);
	//set current textuire we are working on to texture 0
	glActiveTexture(GL_TEXTURE0);
	//bind next tasks to tex0 like in vbos
	glBindTexture(GL_TEXTURE_2D, this->texture);

	//assign loaded texture tp openGL reference
	glTexImage2D(GL_TEXTURE_2D,
		0, //texture 0
		GL_RGBA, // target color format
		img_width,// tex width
		img_height,// tex height
		0,
		GL_RGB, //color format of the texture, use rgba for png, rgb for things that dont ahve an alpha channel
		GL_UNSIGNED_BYTE,
		tex_bytes);  // loaded texture in bytes
	//generate the mipmaps to the current texture
	glGenerateMipmap(GL_TEXTURE_2D);
	//free loaded bytes
	stbi_image_free(tex_bytes);

	//normals setup, this is why we needed a different constructor
	int img_width2, img_height2, colorChannels2;
	unsigned char* normal_bytes = stbi_load(normChar, &img_width2, &img_height2, &colorChannels2, 0);
	GLuint norm_tex;
	glGenTextures(1, &norm_tex);
	glActiveTexture(GL_TEXTURE1);
	//bind next tasks to tex0 like in vbos
	glBindTexture(GL_TEXTURE_2D, norm_tex);
	glTexImage2D(GL_TEXTURE_2D,
		0, //texture 0
		GL_RGB, // target color format
		img_width2,// tex width
		img_height2,// tex height
		0,
		GL_RGB, //color format of the texture, use rgba for png, rgb for things that dont ahve an alpha channel
		GL_UNSIGNED_BYTE,
		normal_bytes);  // loaded texture in bytes
	//generate the mipmaps to the current texture
	glGenerateMipmap(GL_TEXTURE_2D);
	//free loaded bytes
	stbi_image_free(normal_bytes);




	//enable depth testing
	glEnable(GL_DEPTH_TEST);

	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> material;
	std::string warning, error;

	tinyobj::attrib_t attributes;

	bool success = tinyobj::LoadObj(
		&attributes,
		&shapes,
		&material,
		&warning,
		&error,
		objStr.c_str()
	);

	std::vector<glm::vec3> tangents;
	std::vector<glm::vec3> bitangents;
	for (int i = 0; i < shapes[0].mesh.indices.size(); i += 3) {
		tinyobj::index_t vData1 = shapes[0].mesh.indices[i];
		tinyobj::index_t vData2 = shapes[0].mesh.indices[i + 1];
		tinyobj::index_t vData3 = shapes[0].mesh.indices[i + 2];

		glm::vec3 v1 = glm::vec3(
			attributes.vertices[(vData1.vertex_index * 3)],
			attributes.vertices[(vData1.vertex_index * 3) + 1],
			attributes.vertices[(vData1.vertex_index * 3) + 2]
		);
		glm::vec3 v2 = glm::vec3(
			attributes.vertices[(vData2.vertex_index * 3)],
			attributes.vertices[(vData2.vertex_index * 3) + 1],
			attributes.vertices[(vData2.vertex_index * 3) + 2]
		);
		glm::vec3 v3 = glm::vec3(
			attributes.vertices[(vData3.vertex_index * 3)],
			attributes.vertices[(vData3.vertex_index * 3) + 1],
			attributes.vertices[(vData3.vertex_index * 3) + 2]
		);

		glm::vec2 uv1 = glm::vec2(
			attributes.texcoords[(vData1.texcoord_index * 2)],
			attributes.texcoords[(vData1.texcoord_index * 2) + 1]
		);
		glm::vec2 uv2 = glm::vec2(
			attributes.texcoords[(vData2.texcoord_index * 2)],
			attributes.texcoords[(vData2.texcoord_index * 2) + 1]
		);
		glm::vec2 uv3 = glm::vec2(
			attributes.texcoords[(vData3.texcoord_index * 2)],
			attributes.texcoords[(vData3.texcoord_index * 2) + 1]
		);

		glm::vec3 deltaPos1 = v2 - v1;
		glm::vec3 deltaPos2 = v3 - v1;

		glm::vec2 deltaUV1 = uv2 - uv1;
		glm::vec2 deltaUV2 = uv3 - uv1;

		float r = 1.f / ((deltaUV1.x * deltaUV2.y) - (deltaUV1.y * deltaUV2.x));
		glm::vec3 tangent = (deltaPos1 * deltaUV2.y - deltaPos2 * deltaUV1.y) * r;
		glm::vec3 bitangent = (deltaPos2 * deltaUV1.x - deltaPos1 * deltaUV2.x) * r;

		tangents.push_back(tangent);
		tangents.push_back(tangent);
		tangents.push_back(tangent);

		bitangents.push_back(bitangent);
		bitangents.push_back(bitangent);
		bitangents.push_back(bitangent);

		std::vector<GLuint> mesh_indices;
		for (int i = 0; i < shapes[0].mesh.indices.size(); i++) {
			tinyobj::index_t vData = shapes[0].mesh.indices[i];

			this->fullVertexData.push_back(//x
				attributes.vertices[(vData.vertex_index * 3)]
			);
			this->fullVertexData.push_back(//y
				attributes.vertices[(vData.vertex_index * 3) + 1]
			);
			this->fullVertexData.push_back(//z
				attributes.vertices[(vData.vertex_index * 3) + 2]
			);
			this->fullVertexData.push_back(//x
				attributes.normals[(vData.normal_index * 3)]
			);
			this->fullVertexData.push_back(//y
				attributes.normals[(vData.normal_index * 3) + 1]
			);
			this->fullVertexData.push_back(//z
				attributes.normals[(vData.normal_index * 3) + 2]
			);
			this->fullVertexData.push_back(//u
				attributes.texcoords[(vData.texcoord_index * 2)]
			);
			this->fullVertexData.push_back(//v
				attributes.texcoords[(vData.texcoord_index * 2) + 1]
			);
			this->fullVertexData.push_back(//tang
				tangents[i].x
			);
			this->fullVertexData.push_back(
				tangents[i].y
			);
			this->fullVertexData.push_back(
				tangents[i].z
			);
			this->fullVertexData.push_back( //bi
				bitangents[i].x
			);
			this->fullVertexData.push_back(
				bitangents[i].y
			);
			this->fullVertexData.push_back(
				bitangents[i].z
			);
		}
		//std::cout << this->fullVertexData.size() << std::endl;

		glGenVertexArrays(1, &this->VAO); //assigns an id to var
		glGenBuffers(1, &this->VBO);


		//tell opengl to use the shaderprog for the vaos
		glUseProgram(shaderProg);
		// editing VAO = null=
		glBindVertexArray(this->VAO);
		// editing VBO = null

		glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
		glBufferData(
			GL_ARRAY_BUFFER,
			sizeof(GLfloat) * this->fullVertexData.size(),
			this->fullVertexData.data(),
			GL_DYNAMIC_DRAW
		);

		glVertexAttribPointer(
			0,
			3,
			GL_FLOAT,
			GL_FALSE,
			14 * sizeof(float),
			(void*)0
		);


		glEnableVertexAttribArray(0); //id given to pointer index

		//since lit starts at index 3 or index 4 of vert data
		GLintptr litPtr = 3 * sizeof(float);
		glVertexAttribPointer(
			1,
			3,
			GL_FLOAT, GL_FALSE,
			14 * sizeof(float),
			(void*)litPtr
		);
		glEnableVertexAttribArray(1);

		//since UV starts at index 5 or index 6 of vert data
		GLintptr uvPtr = 6 * sizeof(float);

		glVertexAttribPointer(
			2,
			2,
			GL_FLOAT, GL_FALSE,
			14 * sizeof(float),
			(void*)uvPtr
		);

		glEnableVertexAttribArray(2);


		//tangents
		GLintptr tangentPtr = 8 * sizeof(float);

		glVertexAttribPointer(
			3,
			3,
			GL_FLOAT, GL_FALSE,
			14 * sizeof(float),
			(void*)tangentPtr
		);

		glEnableVertexAttribArray(3);

		//tangents
		GLintptr bitangentPtr = 11 * sizeof(float);

		glVertexAttribPointer(
			4,
			3,
			GL_FLOAT, GL_FALSE,
			14 * sizeof(float),
			(void*)bitangentPtr
		);

		glEnableVertexAttribArray(4);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}
}

Model::Model(GLFWwindow* window, glm::vec3 vecPos, GLuint shaderProg, std::string objStr, std::string texStr, float scale, glm::vec3 color) {
	this->scale = scale;
	this->thetaX = 0.f;
	this->thetaY = 0.f;
	this->thetaZ = 0.f;
	//pitch is vert yaw is hori
	this->pitch = 0;
	this->yaw = 90;
	bool isDestroyed = false;
	this->color = color;

	this->x = vecPos[0];
	this->y = vecPos[1];
	this->z = vecPos[2];

	this->opacity = 1.0f;

	//convert the strings to const char for stbi
	const char* texChar = texStr.c_str();

	//TEXTURING
	int img_width, img_height, colorChannels;

	stbi_set_flip_vertically_on_load(true);
	unsigned char* tex_bytes = stbi_load(texChar, &img_width, &img_height, &colorChannels, 0);


	//generate a new reference
	glGenTextures(1, &this->texture);
	//set current textuire we are working on to texture 0
	glActiveTexture(GL_TEXTURE0);
	//bind next tasks to tex0 like in vbos
	glBindTexture(GL_TEXTURE_2D, this->texture);

	//assign loaded texture tp openGL reference
	glTexImage2D(GL_TEXTURE_2D,
		0, //texture 0
		GL_RGBA, // target color format
		img_width,// tex width
		img_height,// tex height
		0,
		GL_RGB, //color format of the texture, use rgba for png, rgb for things that dont ahve an alpha channel
		GL_UNSIGNED_BYTE,
		tex_bytes);  // loaded texture in bytes
	//generate the mipmaps to the current texture
	glGenerateMipmap(GL_TEXTURE_2D);
	//free loaded bytes
	stbi_image_free(tex_bytes);



	//enable depth testing
	glEnable(GL_DEPTH_TEST);

	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> material;
	std::string warning, error;

	tinyobj::attrib_t attributes;

	bool success = tinyobj::LoadObj(
		&attributes,
		&shapes,
		&material,
		&warning,
		&error,
		objStr.c_str()
	);


	std::vector<GLuint> mesh_indices;
	for (int i = 0; i < shapes[0].mesh.indices.size(); i++) {
		tinyobj::index_t vData = shapes[0].mesh.indices[i];

		this->fullVertexData.push_back(//x
			attributes.vertices[(vData.vertex_index * 3)]
		);
		this->fullVertexData.push_back(//y
			attributes.vertices[(vData.vertex_index * 3) + 1]
		);
		this->fullVertexData.push_back(//z
			attributes.vertices[(vData.vertex_index * 3) + 2]
		);
		this->fullVertexData.push_back(//x
			attributes.normals[(vData.normal_index * 3)]
		);
		this->fullVertexData.push_back(//y
			attributes.normals[(vData.normal_index * 3) + 1]
		);
		this->fullVertexData.push_back(//z
			attributes.normals[(vData.normal_index * 3) + 2]
		);
		this->fullVertexData.push_back(//u
			attributes.texcoords[(vData.texcoord_index * 2)]
		);
		this->fullVertexData.push_back(//v
			attributes.texcoords[(vData.texcoord_index * 2) + 1]
		);

	}
	//std::cout << this->fullVertexData.size() << std::endl;

	glGenVertexArrays(1, &this->VAO); //assigns an id to var
	glGenBuffers(1, &this->VBO);


	//tell opengl to use the shaderprog for the vaos
	glUseProgram(shaderProg);
	// editing VAO = null=
	glBindVertexArray(this->VAO);
	// editing VBO = null

	glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
	glBufferData(
		GL_ARRAY_BUFFER,
		sizeof(GLfloat) * this->fullVertexData.size(),
		this->fullVertexData.data(),
		GL_DYNAMIC_DRAW
	);

	glVertexAttribPointer(
		0,
		3,
		GL_FLOAT,
		GL_FALSE,
		8 * sizeof(float),
		(void*)0
	);


	glEnableVertexAttribArray(0); //id given to pointer index

	//since lit starts at index 3 or index 4 of vert data
	GLintptr litPtr = 3 * sizeof(float);
	glVertexAttribPointer(
		1,
		3,
		GL_FLOAT, GL_FALSE,
		8 * sizeof(float),
		(void*)litPtr
	);
	glEnableVertexAttribArray(1);

	//since UV starts at index 5 or index 6 of vert data
	GLintptr uvPtr = 6 * sizeof(float);

	glVertexAttribPointer(
		2,
		2,
		GL_FLOAT, GL_FALSE,
		8 * sizeof(float),
		(void*)uvPtr
	);

	glEnableVertexAttribArray(2);

}

void Model::explodeAndDie() {
	glDeleteVertexArrays(1, &this->VAO);
	glDeleteBuffers(1, &this->VBO);
}

void Model::AddForce(P6::MyVector force) {
	this->accumulatedForce += force;
}

void Model::ResetForce() {
	this->accumulatedForce = P6::MyVector(0, 0, 0);
	this->Acceleration = P6::MyVector(0, 0, 0);
}

void Model::Position(float x, float y, float z) {
	this->x = x;
	this->y = y;
	this->z = z;
}
void Model::UpdatePosition(float time) {
	this->x += this->Velocity.x * time + (0.5f * this->Acceleration.x * (time * time));
	this->y += this->Velocity.y * time + (0.5f * this->Acceleration.y * (time * time));
	this->z += this->Velocity.z * time + (0.5f * this->Acceleration.z * (time * time));
	//std::cout << this->y << std::endl;
}

void Model::UpdateVelocity(float time) {
	this->Acceleration += accumulatedForce * (1 / Mass);

	this->Velocity = this->Velocity + (this->Acceleration * time);

	this->Velocity = this->Velocity * powf(damping, time);
}

void Model::Update(float time) {
	UpdateVelocity(time);
	UpdatePosition(time);
	//std::cout << this->x << " " << this->y << " " << this->z << std::endl;
	this->ResetForce();

	this->currTime += time;
}

void Model::Destroy() {
	this->isDestroyed = true;



	this->Velocity = { 0,0,0 };
	this->Acceleration = { 0,0,0 };

}

void Model::processEvents(glm::mat4 identity_matrix4,
	GLuint shaderProg,
	MyCamera* Camera,
	PointLight* Point, DirectionLight* Direction) {
	this->draw(identity_matrix4,
		shaderProg,
		Camera,
		Point, Direction);
}

void Model::draw(glm::mat4 identity_matrix4,
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
	Point->updatePos(glm::vec3(transformation_matrix[3][0] += 12 * this->objDir.x, transformation_matrix[3][1], transformation_matrix[3][2]));

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
