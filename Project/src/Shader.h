#pragma once

#include <iostream>
#include <string>
#include <fstream>

#include <GL\glew.h>

class Shader {
private:
	unsigned int shaderID, uniformProjection, uniformModel;
public:
	Shader();
	~Shader();

	void CreateFromString(const std::string& vertexCode, const std::string& fragmentCode);

	void CreateFromFiles(const std::string vertexLocation, const std::string fragmentLocation);

	std::string ReadFile(const std::string fileLocation);

	unsigned int GetProjectionLocation() const;
	unsigned int GetModelLocation() const;

	void UseShader();
	void ClearShader();

private:
	void CompileShader(const std::string& vertexCode,const std::string& fragmentCode);
	void AddShader(const char* shaderCode, GLenum shaderType);
};