#ifndef vertextData
#define vertexData

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/gl.h> // include glad to get all the required OpenGL headers
#include <iostream>


class VertexData {
	private:
		unsigned int id;
		unsigned int indicesNum;
	public:
		VertexData(float vertex[], int vertexSize, float indices[], int indicesSize);
		void render();
};
#endif