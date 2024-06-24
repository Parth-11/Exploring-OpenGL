#include "Shader.h"

Shader::Shader()
{
	shaderID = 0;
	uniformModel = 0;
	uniformProjection = 0;
}

Shader::~Shader()
{
	ClearShader();
}

void Shader::CreateFromString(const std::string& vertexCode, const std::string& fragmentCode)
{
	CompileShader(vertexCode, fragmentCode);
}

std::string Shader::ReadFile(const std::string fileLocation) 
{
	std::string content;
	std::ifstream fileStream(fileLocation,std::ios::in);

	if (!fileStream.is_open()) {
		std::cout << "Falied to read " << fileLocation << " file doesn't exit" << std::endl;
		return "";
	}

	std::string line = "";
	while (!fileStream.eof())
	{
		std::getline(fileStream, line);
		content.append(line + "\n");
	}

	fileStream.close();
	return content;
}

void Shader::CreateFromFiles(const std::string vertexLocation, const std::string fragmentLocation)
{
	std::string vShader = ReadFile(vertexLocation);
	std::string fShader = ReadFile(fragmentLocation);

	CompileShader(vShader.c_str(), fShader.c_str());
}

unsigned int Shader::GetProjectionLocation() const
{
	return uniformProjection;
}

unsigned int Shader::GetModelLocation() const
{
	return uniformModel;
}

void Shader::UseShader()
{
	glUseProgram(shaderID);
}

void Shader::ClearShader()
{
	if (shaderID != 0) {
		glDeleteProgram(shaderID);
		shaderID = 0;
	}

	uniformModel = 0;
	uniformProjection = 0;
}

void Shader::CompileShader(const std::string& vertexCode, const std::string& fragmentCode)
{
	shaderID = glCreateProgram();

	if (!shaderID) {
		std::cout << "Error creating shader program" << std::endl;
		return;
	}

	AddShader(vertexCode.c_str(), GL_VERTEX_SHADER);
	AddShader(fragmentCode.c_str(), GL_FRAGMENT_SHADER);

	int result = 0;
	char eLog[1024] = { 0 };

	glLinkProgram(shaderID);
	glGetProgramiv(shaderID, GL_LINK_STATUS, &result);

	if (!result) {
		glGetProgramInfoLog(shaderID, sizeof(eLog), NULL, eLog);
		std::cout << "Error linking program" << eLog << std::endl;
		return;
	}

	glValidateProgram(shaderID);
	glGetProgramiv(shaderID, GL_VALIDATE_STATUS, &result);

	if (!result) {
		glGetProgramInfoLog(shaderID, sizeof(eLog), NULL, eLog);
		std::cout << "Error validating program" << eLog << std::endl;
		return;
 	}

	uniformModel = glGetUniformLocation(shaderID, "model");
	uniformProjection = glGetUniformLocation(shaderID, "projection");
}

void Shader::AddShader(const char* shaderCode, GLenum shaderType)
{
	unsigned int shader = glCreateShader(shaderType);

	const char* code[1];
	code[0] = shaderCode;

	int length[1];
	length[0] = strlen(shaderCode);

	glShaderSource(shader, 1, code, length);
	glCompileShader(shader);

	int result = 0;
	char eLog[1024] = { 0 };

	glGetShaderiv(shader, GL_COMPILE_STATUS, &result);
	if (!result) {
		glGetShaderInfoLog(shader, sizeof(eLog), NULL, eLog);
		std::cout << "Error Compiling the " << shaderType << " shader:" << eLog << std::endl;
		return;
	}
	glAttachShader(shaderID, shader);
}
