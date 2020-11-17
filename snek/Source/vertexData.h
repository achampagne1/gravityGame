#ifndef vertextData
#define vertexData

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/gl.h> // include glad to get all the required OpenGL headers
#include <fstream>
#include <sstream>
#include <iostream>


class VertexData {
	private:
		unsigned int VAO,VBO,EBO;
		unsigned int indicesNum;
	public:
		VertexData(const char* modelPath);
		void render();
		void destroy();
};
#endif