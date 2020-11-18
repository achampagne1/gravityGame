#ifndef vertextData
#define vertexData

#define GLFW_INCLUDE_NONE
#include "loadFile.h"
#include "convertToFloat.h"
#include <GLFW/glfw3.h>
#include <glad/gl.h> // include glad to get all the required OpenGL headers


class VertexData {
	private:
		unsigned int VAO,VBO,EBO;
		int width = 0;
		int height = 0;
	public:
		VertexData(const char* modelPath,int width,int height);
		void render();
		void destroy();
};
#endif