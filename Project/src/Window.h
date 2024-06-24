#pragma once

#include <iostream>
#include <GL\glew.h>
#include <GLFW\glfw3.h>

class Window {
private:
	GLFWwindow* mainWindow;
	GLint width, height;
	GLint bufferWidth, bufferHeight;
public:
	Window();
	Window(GLint windowWidth, GLint windowHeight);

	GLfloat getBufferWidth() const { return bufferWidth; }
	GLfloat getBufferHeight() const { return bufferHeight; }

	bool getShouldClose() const { return glfwWindowShouldClose(mainWindow); }

	int Initialise();

	void swapBuffers() { glfwSwapBuffers(mainWindow); }

	~Window();
};