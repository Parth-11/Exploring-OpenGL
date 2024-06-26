#pragma once

#include <iostream>
#include <GL\glew.h>
#include <GLFW\glfw3.h>

class Window {
private:
	GLFWwindow* mainWindow;
	GLint width, height;
	GLint bufferWidth, bufferHeight;

	bool* keys;

	float last[2];
	float change[2];
	bool mouseFirstMoved;

	void createCallbacks();

	static void handleKeys(GLFWwindow* window, int key, int code, int action, int mode);
	static void handleMouse(GLFWwindow* window, double xPos, double yPos);
public:
	Window();
	Window(GLint windowWidth, GLint windowHeight);

	GLfloat getBufferWidth() const { return bufferWidth; }
	GLfloat getBufferHeight() const { return bufferHeight; }

	bool* getKeys() const { return keys; }
	float* getChange();

	bool getShouldClose() const { return glfwWindowShouldClose(mainWindow); }

	int Initialise();

	void swapBuffers() { glfwSwapBuffers(mainWindow); }

	~Window();
};