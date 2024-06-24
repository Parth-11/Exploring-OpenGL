#pragma once

#include <GL\glew.h>

class Mesh {
private:
	unsigned int VAO, VBO, IBO;
	int indexCount;
public:
	Mesh();
	~Mesh();

	void CreateMesh(float *vertices,unsigned int *indices,unsigned int numOfVertices,unsigned int numOfIndices);
	void RenderMesh();
	void ClearMesh();
};