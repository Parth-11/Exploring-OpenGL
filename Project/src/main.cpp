#include <iostream>
#include <string>
#include <GL\glew.h>
#include <GLFW\glfw3.h>

// Window dimensions
const GLint WIDTH = 800, HEIGHT = 600;

GLuint VAO, VBO, shader;

//vertex shader
static const std::string vertexShader = "\n\
#version 330 \n\
layout (location=0) in vec3 pos; \n\
\n\
void main()\n\
{\n\
	gl_Position = vec4(0.4*pos.x,0.4*pos.y,pos.z,1.0f);\n\
}\n\
";
//Fragment Shader
static const std::string fragmentShader = "\n\
#version 330 \n\
\n\
out vec4 color;\n\
\n\
void main()\n\
{\n\
	color = vec4(1.0f,0.0f,0.0f,1.0f);\n\
}\n\
";

void AddShader(GLuint program, const std::string shaderCode, GLenum type) {
	unsigned int theShader = glCreateShader(type);

	const char* code[1];
	code[0] = shaderCode.c_str();

	int length[1];
	length[0] = shaderCode.length();

	glShaderSource(theShader, 1, code, length);
	glCompileShader(theShader);

	GLint result = 0;
	char eLog[1024] = { 0 };

	glGetShaderiv(theShader, GL_COMPILE_STATUS, &result);
	if (!result) {
		glGetShaderInfoLog(theShader, sizeof(eLog), NULL, eLog);
		std::cout << "Error compiling the " << type << " shader: " << eLog << std::endl;
		return;
	}

	glAttachShader(program, theShader);
}

void CompileShader() {
	shader = glCreateProgram();

	if (!shader) {
		std::cout << "Error creating shader program" << std::endl;
		return;
	}
	AddShader(shader, vertexShader, GL_VERTEX_SHADER);
	AddShader(shader, fragmentShader, GL_FRAGMENT_SHADER);

	GLint result = 0;
	GLchar eLog[1024] = { 0 };

	glLinkProgram(shader);
	glGetProgramiv(shader, GL_LINK_STATUS, &result);
	if (!result) {
		glGetProgramInfoLog(shader, sizeof(eLog), NULL, eLog);
		std::cout << "Error linking program:" << eLog << std::endl;
		return;
	}

	glValidateProgram(shader);
	glGetProgramiv(shader, GL_VALIDATE_STATUS, &result);
	if (!result) {
		glGetProgramInfoLog(shader, sizeof(eLog), NULL, eLog);
		std::cout << "Error validating program:" << eLog << std::endl;
		return;
	}

}

void CreateTriangle() {
	GLfloat vertices[] = {
		-1.0f,-1.0f,0.0f,
		 1.0f,-1.0f,0.0f,
		 0.0f, 1.0f,0.0f
	};

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

int main()
{
	// Initialise GLFW
	if (!glfwInit())
	{
		printf("GLFW initialisation failed!");
		glfwTerminate();
		return 1;
	}

	// Setup GLFW window properties
	// OpenGL version
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// Core Profile = No Backwards Compatibility
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	// Allow Forward Compatbility
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	// Create the window
	GLFWwindow* mainWindow = glfwCreateWindow(WIDTH, HEIGHT, "Test Window", NULL, NULL);
	if (!mainWindow)
	{
		printf("GLFW window creation failed!");
		glfwTerminate();
		return 1;
	}

	// Get Buffer Size information
	int bufferWidth, bufferHeight;
	glfwGetFramebufferSize(mainWindow, &bufferWidth, &bufferHeight);

	// Set context for GLEW to use
	glfwMakeContextCurrent(mainWindow);

	// Allow modern extension features
	glewExperimental = GL_TRUE;

	if (glewInit() != GLEW_OK)
	{
		printf("GLEW initialisation failed!");
		glfwDestroyWindow(mainWindow);
		glfwTerminate();
		return 1;
	}

	// Setup Viewport size
	glViewport(0, 0, bufferWidth, bufferHeight);

	CreateTriangle();
	CompileShader();

	// Loop until window closed
	while (!glfwWindowShouldClose(mainWindow))
	{
		// Get + Handle user input events
		glfwPollEvents();

		// Clear window
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(shader);
		glBindVertexArray(VAO);

		glDrawArrays(GL_TRIANGLES, 0, 3);

		glBindVertexArray(0);
		glUseProgram(0);

		glfwSwapBuffers(mainWindow);
	}

	return 0;
}