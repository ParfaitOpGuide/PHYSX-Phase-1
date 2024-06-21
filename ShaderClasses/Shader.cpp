#include "Shader.h"

	Shader::Shader(GLFWwindow* window, std::string vertStr, std::string fragStr) {
		glfwMakeContextCurrent(window);
		gladLoadGL();
		//Load shader file into a strin steam
		std::fstream vertSrc(vertStr);
		std::stringstream vertBuff;


		vertBuff << vertSrc.rdbuf();


		std::string vertS = vertBuff.str();
		const char* v = vertS.c_str();


		//same but for frag
		std::fstream fragSrc(fragStr);
		std::stringstream fragBuff;


		fragBuff << fragSrc.rdbuf();
	

		std::string fragS = fragBuff.str();
		const char* f = fragS.c_str();



		//create vertex shader 
		GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);

		//Assign source to vertex shader
		glShaderSource(vertexShader, 1, &v, NULL);
		glCompileShader(vertexShader);



		//create frag shader 
		GLuint fragShader = glCreateShader(GL_FRAGMENT_SHADER);

		//Assign source to frag shader
		glShaderSource(fragShader, 1, &f, NULL);
		glCompileShader(fragShader);


		//createprogram
		this->shaderProg = glCreateProgram();

		//attach both
		glAttachShader(this->shaderProg, vertexShader);
		glAttachShader(this->shaderProg, fragShader);
	

		//finalize compilation
		glLinkProgram(this->shaderProg);


		//error checking
		GLint isCompiled = 0;
		glGetShaderiv(fragShader, GL_COMPILE_STATUS, &isCompiled);
		if (isCompiled == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetShaderiv(fragShader, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			std::vector<GLchar> errorLog(maxLength);
			glGetShaderInfoLog(fragShader, maxLength, &maxLength, &errorLog[0]);

			std::cout << &errorLog[0]; //show the error

			// Provide the infolog in whatever manor you deem best.
			// Exit with failure.
			glDeleteShader(fragShader); // Don't leak the shader.

		}

		isCompiled = 0;
		glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &isCompiled);
		if (isCompiled == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			std::vector<GLchar> errorLog(maxLength);
			glGetShaderInfoLog(vertexShader, maxLength, &maxLength, &errorLog[0]);

			std::cout << &errorLog[0]; //show the error

			// Provide the infolog in whatever manor you deem best.
			// Exit with failure.
			glDeleteShader(vertexShader); // Don't leak the shader.

		}
	}
