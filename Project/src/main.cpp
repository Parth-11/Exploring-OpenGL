#include <iostream>
#include <string>
#include <cmath>
#include <vector>
#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>
#include "Window.h"
#include "Mesh.h"
#include "Shader.h"
#include "Camera.h"
#include "Texture.h"
#include "Light.h"

const float toRadians = 3.14159265f / 180.0f;

std::vector<Mesh*> meshList;
std::vector<Shader> shaderList;
Window mainWindow = Window(800, 600);

Texture brickTexture;
Texture dirtTexture;
Light mainLight;

float deltaTime = 0.0f;
float lastTime = 0.0f;

static const std::string vShader = "Shaders/shader.vert.txt";
static const std::string fShader = "Shaders/shader.frag.txt";

void calcAverageNormals(unsigned int* indices, unsigned int indiceCount, float* vertices, unsigned int verticeCount, unsigned int vLength, unsigned int normalOffset)
{
	for (size_t i{ 0 }; i < indiceCount; i+=3)
	{
		unsigned int in0 = indices[i] * vLength;
		unsigned int in1 = indices[i + 1] * vLength;
		unsigned int in2 = indices[i + 2] * vLength;

		glm::vec3 v1(vertices[in1] - vertices[in0], vertices[in1 + 1] - vertices[in0 + 1], vertices[in1 + 2] - vertices[in0 + 2]);
		glm::vec3 v2(vertices[in2] - vertices[in0], vertices[in1 + 1] - vertices[in0 + 1], vertices[in2 + 2] - vertices[in0 + 2]);
		glm::vec3 normal = glm::cross(v1, v2);
		normal = glm::normalize(normal);

		in0 += normalOffset;
		in1 += normalOffset;
		in2 += normalOffset;

		vertices[in0] += normal.x;
		vertices[in0 + 1] += normal.y;
		vertices[in0 + 2] += normal.z;

		vertices[in1] += normal.x;
		vertices[in1 + 1] += normal.y;
		vertices[in1 + 2] += normal.z;

		vertices[in2] += normal.x;
		vertices[in2 + 1] += normal.y;
		vertices[in2 + 2] += normal.z;
	}

	for (size_t i{ 0 }; i < verticeCount / vLength; i++)
	{
		unsigned int nOffset = i * vLength + normalOffset;
		glm::vec3 vec(vertices[nOffset], vertices[nOffset + 1], vertices[nOffset + 2]);
		vec = glm::normalize(vec);
		vertices[nOffset] = vec.x;
		vertices[nOffset + 1] = vec.y;
		vertices[nOffset + 2] = vec.z;
	}
}

void CreateShaders() {
	Shader* shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);
}

void CreateObjects() {

	unsigned int indices[] = {
		0,3,1,
		1,3,2,
		2,3,0,
		0,1,2
	};

	float vertices[] = {
	//	  x		 y	 z		u	v	nx   ny   nz
		-1.0f,-1.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,
		 0.0f,-1.0f,1.0f,0.5f,0.0f,0.0f,0.0f,0.0f,
		 1.0f,-1.0f,0.0f,1.0f,0.0f,0.0f,0.0f,0.0f,
		 0.0f, 1.0f,0.0f,0.5f,1.0f,0.0f,0.0f,0.0f,
	};

	calcAverageNormals(indices, 12, vertices, 32, 8, 5);

	Mesh* obj1 = new Mesh();

	obj1->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj1);

	Mesh* obj2 = new Mesh();
	obj2->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj2);
}

int main()
{
	mainWindow.Initialise();

	CreateObjects();
	CreateShaders();

	Camera camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, 0.0f, 5.0f, 0.5f);
	mainLight = Light(1.0f,1.0f,1.0f,0.2f,2.0f,-1.0f,-2.0f,1.0f);

	brickTexture = Texture("Textures/brick.png");
	brickTexture.LoadTexture();
	dirtTexture = Texture("Textures/dirt.png");
	dirtTexture.LoadTexture();

	GLfloat aspect = (mainWindow.getBufferWidth() / mainWindow.getBufferHeight());

	unsigned int uniformProjection = 0, uniformModel = 0;
	unsigned int uniformView = 0;
	unsigned int uniformAmbientIntensity = 0;
	unsigned int uniformAmbientColour = 0;
	unsigned int uniformDiffuseIntensity = 0;
	unsigned int uniformDirection = 0;

	glm::mat4 projection = glm::perspective(45.0f, aspect, 0.1f, 100.0f);

	// Loop until window closed
	while (!mainWindow.getShouldClose())
	{
		float now = glfwGetTime();
		deltaTime = now - lastTime;
		lastTime = now;

		// Get + Handle user input events
		glfwPollEvents();

		camera.keyControl(mainWindow.getKeys(),deltaTime);
		float* change = mainWindow.getChange();
		camera.mouseControl(change[0],change[1]);
		
		// Clear window
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		shaderList[0].UseShader();
		uniformModel = shaderList[0].GetModelLocation();
		uniformProjection = shaderList[0].GetProjectionLocation();
		uniformView = shaderList[0].GetViewLocation();
		uniformAmbientColour = shaderList[0].GetAmbientColourLocation();
		uniformAmbientIntensity = shaderList[0].GetAmbientIntensityLocation();
		uniformDiffuseIntensity = shaderList[0].GetDiffuseIntensityLocation();
		uniformDirection = shaderList[0].GetDirectionLocation();

		mainLight.UseLight(uniformAmbientIntensity, uniformAmbientColour, uniformDiffuseIntensity, uniformDirection);

		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -2.5f));
		/*model = glm::rotate(model, currAngle * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));*/
		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 1.0f));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		brickTexture.UseTexture();
		meshList[0]->RenderMesh();

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f,1.0f,-2.5f));
		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		dirtTexture.UseTexture();
		meshList[1]->RenderMesh();

		glUseProgram(0);

		mainWindow.swapBuffers();
	}

	return 0;
}